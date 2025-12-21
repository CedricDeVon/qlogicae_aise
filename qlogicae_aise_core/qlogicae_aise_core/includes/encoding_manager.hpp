#pragma once

namespace QLogicaeAiseCore
{
    class EncodingManager
    {
    public:        
        bool setup();

        std::future<bool> setup_async();

        void setup_async(
            QLogicaeCore::Result<std::future<void>>& result
        );

        void setup(
            QLogicaeCore::Result<void>& result
        );

        void setup_async(
            const std::function<void(const bool& result)>& callback
        );

        void setup_async(
            const std::function<void(QLogicaeCore::Result<void>& result)>& callback
        );

        bool terminate();

        std::future<bool> terminate_async();

        void terminate_async(
            QLogicaeCore::Result<std::future<void>>& result
        );

        void terminate(
            QLogicaeCore::Result<void>& result
        );

        std::future<bool> terminate_async(
            const std::function<void(const bool& result)>& callback
        );

        void terminate_async(
            const std::function<void(QLogicaeCore::Result<void>& result)>& callback
        );

        bool load_vocabulary_file_content(
            const std::string_view& vocabulary_file_path,
            const std::size_t& vocabulary_size
        );
        
        void load_vocabulary_file_content(
            QLogicaeCore::Result<bool>& result,
            const std::string_view& vocabulary_file_path,
            const std::size_t& vocabulary_size
        );

        std::vector<float> from_string_to_boc(
            const std::string_view& text
        );

        void from_string_to_boc(
            QLogicaeCore::Result<std::vector<float>>& result,
            const std::string_view& text
        );

        static EncodingManager& get_instance();

        static void get_instance(
            QLogicaeCore::Result<EncodingManager*>& result
        );

    protected:
        EncodingManager();

        ~EncodingManager();

        EncodingManager(
            const EncodingManager& instance
        ) = default;

        EncodingManager(
            EncodingManager&& instance
        ) noexcept = delete;

        EncodingManager& operator = (
            EncodingManager&& instance
        ) = delete;

        EncodingManager& operator = (
            const EncodingManager& instance
        ) = delete;

        std::mutex _mutex;

        int _lut[256];
        
        int _unk_idx { -1 };

        std::size_t _vocabulary_size { 0 };

        int _int_1;

        int _int_2;

        size_t _index_1;
        
        size_t _index_2;
        
        size_t _index_3;
        
        size_t _index_4;

        size_t _index_5;

        size_t _lutsize = 256;
        
        float _float_1;
        
        float _float_2;
        
        float _float_3;
        
        float _float_4;
        
        float _boc_inv;

        float _boc_sum = 0.0f;

        float _inv;
        
        std::string _string_1;
        
        std::vector<float> _boc_collection;
        
        size_t _boc_collection_size;

        std::unordered_map<std::string, int> _temporary_map;

        nlohmann::json _json_file_output;
    };

    inline static EncodingManager& ENCODING_MANAGER =
        EncodingManager::get_instance();
}
