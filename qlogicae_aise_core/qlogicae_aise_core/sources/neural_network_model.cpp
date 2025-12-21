#include "pch.hpp"

#include "../includes/neural_network_model.hpp"

namespace QLogicaeAiseCore
{
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

	NeuralNetworkModel::NeuralNetworkModel():
		_env { ORT_LOGGING_LEVEL_WARNING, "exp" },
		_session_options{},
		_run_options{},
		_memory_info
		{
			Ort::MemoryInfo::CreateCpu(
				OrtArenaAllocator,
				OrtMemTypeDefault)
		}
	{		
	}

	NeuralNetworkModel::~NeuralNetworkModel()
	{
		
	}

	bool NeuralNetworkModel::setup()
	{
		try
		{
			QLogicaeCore::Result<void> result;

			setup(result);

			return result.is_status_safe();
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseCore::NeuralNetworkModel::setup()",
				exception.what()
			);

			return false;
		}
	}

	std::future<bool> NeuralNetworkModel::setup_async()
	{
		std::promise<bool> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this,
			promise = std::move(promise)]() mutable
			{
				promise.set_value(
					setup()
				);
			}
		);

		return future;
	}

	void NeuralNetworkModel::setup_async(
		QLogicaeCore::Result<std::future<void>>& result
	)
	{
		std::promise<void> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this,
			promise = std::move(promise)]() mutable
			{
				QLogicaeCore::Result<void> result;

				setup(
					result
				);

				promise.set_value();
			}
		);

		result.set_to_good_status_with_value(
			std::move(future)
		);
	}

	void NeuralNetworkModel::setup(
		QLogicaeCore::Result<void>& result
	)
	{


		result.set_to_good_status_without_value();
	}

	void NeuralNetworkModel::setup_async(
		const std::function<void(const bool& result)>& callback
	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, callback]() mutable
			{
				callback(
					setup()
				);
			}
		);
	}

	void NeuralNetworkModel::setup_async(
		const std::function<void(QLogicaeCore::Result<void>& result)>& callback
	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, callback]() mutable
			{
				QLogicaeCore::Result<void> result;

				setup(result);

				callback(
					result
				);
			}
		);
	}

	bool NeuralNetworkModel::setup(
		const std::string model_path
	)
	{
		try
		{
			QLogicaeCore::Result<void> result;

			setup(
				result,
				model_path
			);

			return result.is_status_safe();
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseCore::NeuralNetworkModel::setup()",
				exception.what()
			);

			return false;
		}
	}

	std::future<bool> NeuralNetworkModel::setup_async(
		const std::string model_path
	)
	{
		std::promise<bool> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, model_path,
			promise = std::move(promise)]() mutable
			{
				promise.set_value(
					setup(model_path)
				);
			}
		);

		return future;
	}

	void NeuralNetworkModel::setup_async(
		QLogicaeCore::Result<std::future<void>>& result,
		const std::string model_path
	)
	{
		std::promise<void> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, model_path,
			promise = std::move(promise)]() mutable
			{
				QLogicaeCore::Result<void> result;

				setup(
					result,
					model_path
				);

				promise.set_value();
			}
		);

		result.set_to_good_status_with_value(
			std::move(future)
		);
	}

	void NeuralNetworkModel::setup(
		QLogicaeCore::Result<void>& result,
		const std::string model_path
	)
	{						
		_session_options.SetIntraOpNumThreads(1);		
		_session_options.SetGraphOptimizationLevel(
			GraphOptimizationLevel::ORT_ENABLE_ALL);
		
		std::wstring wpath = to_wstring(model_path);
		
		_session = std::make_unique<Ort::Session>(
			_env,
			wpath.c_str(),
			_session_options
		);
		
		_input_name =
			_session->GetInputNameAllocated(
				0, _ort_allocator
			).get();

		_output_name =
			_session->GetOutputNameAllocated(
				0, _ort_allocator
			).get();

		_input_names[0] =
			_input_name.c_str();

		_output_names[0] =
			_output_name.c_str();

		_run_options.SetRunLogVerbosityLevel(0);

		_memory_info =
			Ort::MemoryInfo::CreateCpu(
				OrtArenaAllocator, OrtMemTypeDefault
			);

		result.set_to_good_status_without_value();
	}

	void NeuralNetworkModel::setup_async(
		const std::function<void(const bool& result)>& callback,
		const std::string model_path
	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, model_path, callback]() mutable
			{
				callback(
					setup(model_path)
				);
			}
		);
	}

	void NeuralNetworkModel::setup_async(
		const std::function<void(QLogicaeCore::Result<void>& result)>& callback,
		const std::string model_path
	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, model_path, callback]() mutable
			{
				QLogicaeCore::Result<void> result;

				setup(
					result,
					model_path
				);

				callback(
					result
				);
			}
		);
	}

	bool NeuralNetworkModel::terminate()
	{
		try
		{
			QLogicaeCore::Result<void> result;

			terminate(result);

			return result.is_status_safe();
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseCore::NeuralNetworkModel::terminate()",
				exception.what()
			);

			return false;
		}
	}

	std::future<bool> NeuralNetworkModel::terminate_async()
	{
		std::promise<bool> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this,
			promise = std::move(promise)]() mutable
			{
				promise.set_value(
					terminate()
				);
			}
		);

		return future;
	}

	void NeuralNetworkModel::terminate_async(
		QLogicaeCore::Result<std::future<void>>& result
	)
	{
		std::promise<void> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this,
			promise = std::move(promise)]() mutable
			{
				QLogicaeCore::Result<void> result;

				terminate(
					result
				);

				promise.set_value();
			}
		);

		result.set_to_good_status_with_value(
			std::move(future)
		);
	}

	void NeuralNetworkModel::terminate(
		QLogicaeCore::Result<void>& result
	)
	{
		result.set_to_good_status_without_value();
	}

	void NeuralNetworkModel::terminate_async(
		const std::function<void(const bool& result)>& callback
	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, callback]() mutable
			{
				callback(
					terminate()
				);
			}
		);
	}

	void NeuralNetworkModel::terminate_async(
		const std::function<void(QLogicaeCore::Result<void>& result)>& callback
	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, callback]() mutable
			{
				QLogicaeCore::Result<void> result;

				terminate(result);

				callback(
					result
				);
			}
		);
	}

	double NeuralNetworkModel::predict(
		const std::string& input
	)
	{
		try
		{
			QLogicaeCore::Result<double> result;

			predict(
				result,
				input
			);

			return result.get_value();
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseCore::NeuralNetworkModel::predict()",
				exception.what()
			);

			return 0.0;
		}
	}

	void NeuralNetworkModel::predict(
		QLogicaeCore::Result<double>& result,
		const std::string& input
	)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_boc_collection =
			ENCODING_MANAGER.from_string_to_boc(
				input
			);

		_boc_collection_size =
			_boc_collection.size();

		_shape =
		{
			1,
			static_cast<int64_t>(_boc_collection_size)
		};
		
		_tensor =
			Ort::Value::CreateTensor<float>(
			_memory_info,
			const_cast<float*>(
				_boc_collection.data()
			),
			_boc_collection_size,
			_shape.data(),
			_shape.size()
		);

		result.set_to_good_status_with_value(
			static_cast<double>(
				*(_session->Run(
					_run_options,
					_input_names,
					&_tensor,
					1,
					_output_names,
					1
				)).front()
					.GetTensorMutableData<float>()
			)
		);
	}

	NeuralNetworkModel& NeuralNetworkModel::get_instance()
	{
		QLogicaeCore::Result<NeuralNetworkModel*> result;

		get_instance(
			result
		);

		return *result.get_value();
	}

	void NeuralNetworkModel::get_instance(
		QLogicaeCore::Result<NeuralNetworkModel*>& result
	)
	{
		static NeuralNetworkModel instance;

		result.set_to_good_status_with_value(
			&instance
		);
	}
}
