#include "pch.hpp"

#include "main.hpp"

#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <nanobench.h>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>

#include <nlohmann/json.hpp>
#include <onnxruntime_cxx_api.h>

static std::wstring to_wstring(const std::string& s)
{
    std::wstring w;
    w.reserve(s.size());
    for (unsigned char c : s)
    {
        w.push_back(static_cast<wchar_t>(c));
    }
    return w;
}

class Vocabulary
{
public:
    Vocabulary()
    {
        for (int i = 0; i < 256; ++i)
        {
            lut_[i] = -1;
        }
    }

    bool load(const std::string& vocab_path, const std::size_t vocabulary_size)
    {
        std::ifstream f(vocab_path);
        if (!f.is_open())
        {
            return false;
        }

        nlohmann::json j;
        f >> j;

        std::unordered_map<std::string, int> tmp;
        tmp.reserve(vocabulary_size);

        if (j.is_object())
        {
            for (auto& kv : j.items())
            {
                tmp.emplace(kv.key(), kv.value().get<int>());
            }
        }
        else if (j.is_array())
        {
            for (std::size_t i = 0; i < j.size(); ++i)
            {
                tmp.emplace(j[i].get<std::string>(), static_cast<int>(i));
            }
        }
        else
        {
            return false;
        }

        if (tmp.size() != vocabulary_size)
        {
            return false;
        }

        for (int i = 0; i < 256; ++i)
        {
            lut_[i] = -1;
        }

        unk_idx_ = -1;

        for (auto& kv : tmp)
        {
            const std::string& k = kv.first;
            int idx = kv.second;

            if (k.size() == 1)
            {
                lut_[static_cast<unsigned char>(k[0])] = idx;
            }

            if (k == "<UNK>" || k == "<unk>")
            {
                unk_idx_ = idx;
            }
        }

        vocabulary_size_ = vocabulary_size;
        return true;
    }

    std::vector<float> encode_boc(const std::string& text) const
    {
        std::vector<float> v(vocabulary_size_, 0.0f);

        const int unk = unk_idx_;
        const int* lut = lut_;

        const unsigned char* p = reinterpret_cast<const unsigned char*>(text.data());
        const unsigned char* e = p + text.size();

        while (p + 4 <= e)
        {
            int i0 = lut[p[0]];
            int i1 = lut[p[1]];
            int i2 = lut[p[2]];
            int i3 = lut[p[3]];

            if (i0 >= 0) v[i0] += 1.0f; else if (unk >= 0) v[unk] += 1.0f;
            if (i1 >= 0) v[i1] += 1.0f; else if (unk >= 0) v[unk] += 1.0f;
            if (i2 >= 0) v[i2] += 1.0f; else if (unk >= 0) v[unk] += 1.0f;
            if (i3 >= 0) v[i3] += 1.0f; else if (unk >= 0) v[unk] += 1.0f;

            p += 4;
        }

        while (p < e)
        {
            int idx = lut[*p];
            if (idx >= 0) v[idx] += 1.0f;
            else if (unk >= 0) v[unk] += 1.0f;
            ++p;
        }

        float sum = 0.0f;

        for (std::size_t i = 0; i < v.size(); i += 4)
        {
            float a = v[i];
            float b = (i + 1 < v.size()) ? v[i + 1] : 0.0f;
            float c = (i + 2 < v.size()) ? v[i + 2] : 0.0f;
            float d = (i + 3 < v.size()) ? v[i + 3] : 0.0f;

            sum += a * a + b * b + c * c + d * d;
        }

        float inv = (sum > 0.0f) ? 1.0f / std::sqrt(sum) : 0.0f;

        if (inv != 0.0f)
        {
            for (float& x : v)
            {
                x *= inv;
            }
        }

        return v;
    }

    std::size_t size() const
    {
        return vocabulary_size_;
    }

private:
    int lut_[256];
    int unk_idx_{ -1 };
    std::size_t vocabulary_size_{ 0 };
};

class ExposureModel
{
public:
    ExposureModel(const std::string& model_path)
        : env_{ ORT_LOGGING_LEVEL_WARNING, "exp" },
        session_options_{},
        run_options_{},
        memory_info_{
            Ort::MemoryInfo::CreateCpu(
                OrtArenaAllocator,
                OrtMemTypeDefault)
        }
    {
        session_options_.SetIntraOpNumThreads(1);
        session_options_.SetGraphOptimizationLevel(
            GraphOptimizationLevel::ORT_ENABLE_ALL);

        std::wstring wpath = to_wstring(model_path);

        session_ = std::make_unique<Ort::Session>(
            env_,
            wpath.c_str(),
            session_options_
        );

        input_name_ = session_->GetInputNameAllocated(0, alloc).get();
        output_name_ = session_->GetOutputNameAllocated(0, alloc).get();

        input_names_[0] = input_name_.c_str();
        output_names_[0] = output_name_.c_str();

        run_options_.SetRunLogVerbosityLevel(0);

        memory_info_ =
            Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);;
    }

    double predict(const std::vector<float>& input)
    {
        shape = { 1, static_cast<int64_t>(input.size()) };
        total = input.size();

        Ort::Value tensor = Ort::Value::CreateTensor<float>(
            memory_info_,
            const_cast<float*>(input.data()),
            total,
            shape.data(),
            shape.size());

        outputs = session_->Run(
            run_options_,
            input_names_,
            &tensor,
            1,
            output_names_,
            1);

        raw = outputs.front().GetTensorMutableData<float>();
        return static_cast<double>(*raw);
    }

private:
    float* raw;
    std::array<int64_t, 2> shape;
    std::size_t total;
    std::vector<Ort::Value> outputs;
    Ort::AllocatorWithDefaultOptions alloc;

    Ort::Env env_;
    Ort::SessionOptions session_options_;
    Ort::RunOptions run_options_;
    std::unique_ptr<Ort::Session> session_;

    Ort::MemoryInfo memory_info_;

    std::string input_name_;
    std::string output_name_;

    const char* input_names_[1];
    const char* output_names_[1];
};


int main(int, char**)
{
    const std::string vocab_path = "vocabulary.json";
    const std::string model_path = "model.onnx";

    Vocabulary vocab;
    vocab.load(vocab_path, 97);

    ExposureModel model(model_path);

    std::string sample = "std::string password = \"SecretPassword_1234\"";

    auto encoded = vocab.encode_boc(sample);

    ankerl::nanobench::Bench bench;

    bench.minEpochIterations(100000);

    bench.run("vocab_encode", [&]()
        {
            auto r = vocab.encode_boc(sample);
        });

    bench.run("onnx_predict", [&]()
        {
            double p = model.predict(encoded);
        });

    return 0;
}
