#include "pch.hpp"

#include "../includes/encoding_manager.hpp"

namespace QLogicaeAiseConsole
{
	EncodingManager::EncodingManager()
	{
		setup();
	}

	EncodingManager::~EncodingManager()
	{
		terminate();
	}

	bool EncodingManager::setup()
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
				"QLogicaeAiseConsole::EncodingManager::setup()",
				exception.what()
			);

			return false;
		}
	}

	std::future<bool> EncodingManager::setup_async()
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

	void EncodingManager::setup_async(
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

	void EncodingManager::setup(
		QLogicaeCore::Result<void>& result
	)
	{
		for (int i = 0; i < 256; ++i)
		{
			_lut[i] = -1;
		}

		result.set_to_good_status_without_value();
	}

	std::future<bool> EncodingManager::setup_async(
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

	void EncodingManager::setup_async(
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

	bool EncodingManager::terminate()
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
				"QLogicaeAiseConsole::EncodingManager::terminate()",
				exception.what()
			);

			return false;
		}
	}

	std::future<bool> EncodingManager::terminate_async()
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

	void EncodingManager::terminate_async(
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

	void EncodingManager::terminate(
		QLogicaeCore::Result<void>& result
	)
	{
		result.set_to_good_status_without_value();
	}

	std::future<bool> EncodingManager::terminate_async(
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

	void EncodingManager::terminate_async(
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

	bool EncodingManager::load_vocabulary_file_content(
		const std::string_view& vocabulary_file_path,
		const std::size_t& vocabulary_size
	)
	{
		try
		{
			QLogicaeCore::Result<bool> result;

			load_vocabulary_file_content(
				result,
				vocabulary_file_path,
				vocabulary_size
			);

			return result.get_value();
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::EncodingManager::load_vocabulary_file_content()",
				exception.what()
			);

			return false;
		}
	}

	void EncodingManager::load_vocabulary_file_content(
		QLogicaeCore::Result<bool>& result,
		const std::string_view& vocabulary_file_path,
		const std::size_t& vocabulary_size
	)
	{
		std::ifstream file_output(vocabulary_file_path.data());
		if (!file_output.is_open())
		{
			result.set_to_bad_status_with_value(
				false
			);

			return;
		}

		file_output >> _json_file_output; 

		_index_2 = _json_file_output.size();
		_temporary_map.reserve(vocabulary_size);

		if (_json_file_output.is_object())
		{
			for (auto& temporary_map_item : _json_file_output.items())
			{
				_temporary_map.emplace(temporary_map_item.key(), temporary_map_item.value().get<int>());
			}
		}
		else if (_json_file_output.is_array())
		{
			for (_index_1 = 0; _index_1 < _index_2; ++_index_1)
			{
				_temporary_map.emplace(_json_file_output[_index_1].get<std::string>(), static_cast<int>(_index_1));
			}
		}
		else
		{
			result.set_to_bad_status_with_value(
				false
			);

			return;
		}

		if (_temporary_map.size() != vocabulary_size)
		{
			result.set_to_bad_status_with_value(
				false
			);

			return;
		}

		for (_index_1 = 0; _index_1 < _lutsize; ++_index_1)
		{
			_lut[_index_1] = -1;
		}

		_unk_idx = -1;

		for (auto& temporary_map_item : _temporary_map)
		{
			_string_1 = temporary_map_item.first;
			_int_1 = temporary_map_item.second;

			if (_string_1.size() == 1)
			{
				_lut[static_cast<unsigned char>(_string_1[0])] = _int_1;
			}

			if (_string_1 == "<UNK>" || _string_1 == "<unk>")
			{
				_unk_idx = _int_1;
			}
		}

		_vocabulary_size = vocabulary_size;

		result.set_to_good_status_with_value(
			true
		);
	}

	std::vector<float> EncodingManager::from_string_to_boc(
		const std::string_view& text
	)
	{
		try
		{
			QLogicaeCore::Result<std::vector<float>> result;

			from_string_to_boc(
				result,
				text
			);

			return result.get_value();
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::EncodingManager::from_string_to_boc()",
				exception.what()
			);

			return {};
		}
	}

	void EncodingManager::from_string_to_boc(
		QLogicaeCore::Result<std::vector<float>>& result,
		const std::string_view& text
	)
	{
		_boc_collection = std::vector<float>(_vocabulary_size, 0.0f);
		_boc_collection_size = _boc_collection.size();

		const int* lut = _lut;
		const unsigned char* text_pointer =
			reinterpret_cast<const unsigned char*>(text.data());
		const unsigned char* text_pointer_end =
			text_pointer + text.size();

		while (text_pointer + 4 <= text_pointer_end)
		{
			_index_1 = lut[text_pointer[0]];
			_index_2 = lut[text_pointer[1]];
			_index_3 = lut[text_pointer[2]];
			_index_4 = lut[text_pointer[3]];

			if (_index_1 >= 0)
			{
				_boc_collection[_index_1] += 1.0f;
			}
			else if (_unk_idx >= 0)
			{
				_boc_collection[_unk_idx] += 1.0f;
			}

			if (_index_2 >= 0)
			{
				_boc_collection[_index_2] += 1.0f;
			}
			else if (_unk_idx >= 0)
			{
				_boc_collection[_unk_idx] += 1.0f;
			}

			if (_index_3 >= 0)
			{
				_boc_collection[_index_3] += 1.0f;
			}
			else if (_unk_idx >= 0)
			{
				_boc_collection[_unk_idx] += 1.0f;
			}

			if (_index_4 >= 0)
			{
				_boc_collection[_index_4] += 1.0f;
			}
			else if (_unk_idx >= 0)
			{
				_boc_collection[_unk_idx] += 1.0f;
			}

			text_pointer += 4;
		}

		while (text_pointer < text_pointer_end)
		{
			_index_1 = lut[*text_pointer];
			if (_index_1 >= 0)
			{
				_boc_collection[_index_1] += 1.0f;
			}

			else if (_unk_idx >= 0)
			{
				_boc_collection[_unk_idx] += 1.0f;
			}

			++text_pointer;
		}

		for (_index_1 = 0;
			_index_1 < _boc_collection_size;
			_index_1 += 4
		)
		{
			_float_1 =
				_boc_collection[_index_1];
			_float_2 =
				(_index_1 + 1 < _boc_collection_size) ? _boc_collection[_index_1 + 1] : 0.0f;
			_float_3 =
				(_index_1 + 2 < _boc_collection_size) ? _boc_collection[_index_1 + 2] : 0.0f;
			_float_4 =
				(_index_1 + 3 < _boc_collection_size) ? _boc_collection[_index_1 + 3] : 0.0f;

			_boc_sum +=
				_float_1 * _float_1 +
				_float_2 * _float_2 +
				_float_3 * _float_3 +
				_float_4 * _float_4;
		}

		_boc_inv =
			(_boc_sum > 0.0f) ? 1.0f / std::sqrt(_boc_sum) : 0.0f;

		if (_boc_inv != 0.0f)
		{
			for (float& boc_item : _boc_collection)
			{
				boc_item *= _boc_inv;
			}
		}

		result.set_to_good_status_with_value(
			_boc_collection
		);
	}

	EncodingManager& EncodingManager::get_instance()
	{
		QLogicaeCore::Result<EncodingManager*> result;

		get_instance(
			result
		);

		return *result.get_value();
	}

	void EncodingManager::get_instance(
		QLogicaeCore::Result<EncodingManager*>& result
	)
	{
		static EncodingManager instance;

		result.set_to_good_status_with_value(
			&instance
		);
	}
}
