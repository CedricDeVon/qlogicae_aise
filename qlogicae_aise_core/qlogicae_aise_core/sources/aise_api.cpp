#include "pch.hpp"

#include "../includes/aise_api.hpp"

namespace QLogicaeAiseCore
{
	AiseApi::AiseApi()
	{
		setup();
	}

	AiseApi::~AiseApi()
	{
		terminate();
	}

	bool AiseApi::setup()
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
				"QLogicaeAiseCore::AiseApi::setup()",
				exception.what()
			);

			return false;
		}
	}

	std::future<bool> AiseApi::setup_async()
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

	void AiseApi::setup_async(
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

	void AiseApi::setup(
		QLogicaeCore::Result<void>& result
	)
	{
		result.set_to_good_status_without_value();
	}

	std::future<bool> AiseApi::setup_async(
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

	void AiseApi::setup_async(
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

	bool AiseApi::setup(
		const AiseApiConfigurations& configurations
	)
	{
		try
		{
			QLogicaeCore::Result<void> result;

			setup(
				result,
				configurations
			);

			return result.is_status_safe();
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseCore::AiseApi::setup()",
				exception.what()
			);

			return false;
		}
	}

	std::future<bool> AiseApi::setup_async(
		const AiseApiConfigurations& configurations
	)
	{
		std::promise<bool> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, configurations,
			promise = std::move(promise)]() mutable
			{
				promise.set_value(
					setup(configurations)
				);
			}
		);

		return future;
	}

	void AiseApi::setup_async(
		QLogicaeCore::Result<std::future<void>>& result,
		const AiseApiConfigurations& configurations
	)
	{
		std::promise<void> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, configurations,
			promise = std::move(promise)]() mutable
			{
				QLogicaeCore::Result<void> result;

				setup(
					result,
					configurations
				);

				promise.set_value();
			}
		);

		result.set_to_good_status_with_value(
			std::move(future)
		);
	}

	void AiseApi::setup(
		QLogicaeCore::Result<void>& result,
		const AiseApiConfigurations& configurations
	)
	{
		_configurations = configurations;

		result.set_to_good_status_without_value();
	}

	std::future<bool> AiseApi::setup_async(
		const std::function<void(const bool& result)>& callback,
		const AiseApiConfigurations& configurations
	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, configurations, callback]() mutable
			{
				callback(
					setup(configurations)
				);
			}
		);
	}

	void AiseApi::setup_async(
		const std::function<void(QLogicaeCore::Result<void>& result)>& callback,
		const AiseApiConfigurations& configurations
	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, configurations, callback]() mutable
			{
				QLogicaeCore::Result<void> result;

				setup(
					result,
					configurations
				);

				callback(
					result
				);
			}
		);
	}

	bool AiseApi::terminate()
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
				"QLogicaeAiseCore::AiseApi::terminate()",
				exception.what()
			);

			return false;
		}
	}

	std::future<bool> AiseApi::terminate_async()
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

	void AiseApi::terminate_async(
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

	void AiseApi::terminate(
		QLogicaeCore::Result<void>& result
	)
	{
		result.set_to_good_status_without_value();
	}

	std::future<bool> AiseApi::terminate_async(
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

	void AiseApi::terminate_async(
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

	AiseApiFileSystemEvaluationResults AiseApi::evaluate(
		const AiseApiFileSystemEvaluationConfigurations& configurations
	)
	{
		try
		{
			QLogicaeCore::Result<AiseApiFileSystemEvaluationResults> result;

			evaluate(
				result,
				configurations
			);

			return result.get_value();
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseCore::AiseApi::evaluate()",
				exception.what()
			);

			return {};
		}		
	}

	void AiseApi::evaluate(
		QLogicaeCore::Result<AiseApiFileSystemEvaluationResults>& result,
		const AiseApiFileSystemEvaluationConfigurations& configurations
	)
	{
		std::string
			file_extension,
			file_system_entity_path_string;
		
		std::mutex
			results_mutex;

		bool
			extension_matched;

		double
			minimum_positive_prediction =
				configurations.minimum_positive_prediction,
			maximum_positive_prediction =
				configurations.maximum_positive_prediction;
		
		std::filesystem::path
			file_system_entity_path;

		std::vector<std::future<void>>
			future_collection;
		
		AiseApiFileSystemEvaluationResults
			file_system_evaluation_results;
		
		AiseApiFileSystemEvaluationCallbackConfigurationsResults
			callback_configurations_results;		

		NeuralNetworkModel& neural_network_model =
			NeuralNetworkModel::get_instance();

		callback_configurations_results.text = "  Evaluating Files";
		configurations.file_evaluation_callback(
			callback_configurations_results
		);

		if (!std::filesystem::exists(configurations.root_folder_path))
		{
			file_system_evaluation_results.timestamp_end =
				QLogicaeCore::TIME.now();
			result.set_to_good_status_with_value(
				file_system_evaluation_results
			);
			return;
		}		

		for (const auto& file_system_entity :
			std::filesystem::recursive_directory_iterator(
				configurations.root_folder_path,
			std::filesystem::directory_options::skip_permission_denied)
		)
		{
			if (!file_system_entity.is_regular_file())
			{
				++file_system_evaluation_results.total_folders_found_count;
				continue;
			}

			++file_system_evaluation_results.total_files_found_count;

			file_system_entity_path =
				file_system_entity.path();
			file_extension =
				file_system_entity_path.extension().string();
			file_system_entity_path_string =
				file_system_entity_path.string();

			extension_matched =
				!configurations.expected_file_extensions.size();
			for (const auto& expected_file_extension :
				configurations.expected_file_extensions
				)
			{
				if (file_extension == expected_file_extension)
				{
					extension_matched = true;
					break;
				}
			}

			if (!extension_matched)
			{
				continue;
			}			

			std::promise<void> file_promise;
			future_collection.emplace_back(
				file_promise.get_future()
			);

			boost::asio::post(
				QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
				[this,
				&results_mutex,
				&configurations,
				&neural_network_model,
				&minimum_positive_prediction,
				&maximum_positive_prediction,
				&file_system_evaluation_results,
				file_system_entity_path_string,
				file_promise = std::move(file_promise)]() mutable
				{
					std::lock_guard<std::mutex>
						lock(results_mutex);

					BOOL
						is_file_readable;

					size_t
						start,
						length,
						index_1,
						line_number = 0;

					std::string
						line_text,
						leftover;

					auto& file_result =
						file_system_evaluation_results
							.file_evaluation_results[file_system_entity_path_string];

					DWORD
						bytes_read;

					double
						line_prediction;

					AiseApiFileLineEvaluationResults
						file_line_evaluation_results;

					fast_io::native_file file(
						file_system_entity_path_string,
						fast_io::open_mode::in
					);

					HANDLE
						handle = file.native_handle();

					constexpr std::size_t
						buffer_size = 1024;

					char buffer[buffer_size];

					while (true)
					{
						bytes_read = 0;
						is_file_readable = ReadFile(
							handle,
							buffer,
							static_cast<DWORD>(buffer_size),
							&bytes_read, nullptr
						);
						
						if (!is_file_readable || bytes_read == 0)
						{
							break;
						}

						start = 0;
						for (index_1 = 0;
							index_1 < bytes_read;
							++index_1)
						{
							if (buffer[index_1] == '\n')
							{
								length = index_1 - start;

								line_text = leftover;
								leftover.clear();

								if (length > 0 && buffer[start + length - 1] == '\r')
								{
									--length;
								}

								line_text.append(
									buffer + start,
									length
								);

								line_prediction =
									neural_network_model.predict(
										line_text
									);
								file_line_evaluation_results.line_text =
									line_text;
								file_line_evaluation_results.line_number =
									++line_number;
								file_line_evaluation_results.line_prediction =
									line_prediction;
								file_line_evaluation_results.timestamp_end =
									QLogicaeCore::TIME.now();
								if (minimum_positive_prediction <= line_prediction &&
									line_prediction <= maximum_positive_prediction
									)
								{
									++file_system_evaluation_results.total_positive_prediction_count;
									file_system_evaluation_results
										.positive_file_evaluation_results[file_system_entity_path_string]
										.file_line_evaluation_results
										.push_back(
											file_line_evaluation_results
										);
								}

								file_result
									.file_line_evaluation_results
									.push_back(
										file_line_evaluation_results
									);

								start = index_1 + 1;
							}
						}

						if (start < bytes_read)
						{
							leftover.append(buffer + start, bytes_read - start);
						}
					}

					if (!leftover.empty())
					{
						line_prediction =
							neural_network_model.predict(
								leftover
							);
						file_line_evaluation_results.line_text =
							leftover;
						file_line_evaluation_results.line_number =
							++line_number;
						file_line_evaluation_results.line_prediction =
							line_prediction;
						file_line_evaluation_results.timestamp_end =
							QLogicaeCore::TIME.now();
						if (minimum_positive_prediction <= line_prediction &&
							line_prediction <= maximum_positive_prediction
							)
						{
							++file_system_evaluation_results.total_positive_prediction_count;
							file_system_evaluation_results
								.positive_file_evaluation_results[file_system_entity_path_string]
								.file_line_evaluation_results
								.push_back(
									file_line_evaluation_results
								);
						}

						file_result
							.file_line_evaluation_results
							.push_back(
								file_line_evaluation_results
							);
					}						
					
					file_system_evaluation_results.total_line_count += 
						file_result.file_line_evaluation_results.size();

					file_result.timestamp_end =
						QLogicaeCore::TIME.now();
					
					file_promise.set_value();
				}
			);
		}

		for (auto& current_future : future_collection)
		{			
			current_future.get();
			configurations.file_evaluation_callback(
				callback_configurations_results
			);
		}

		file_system_evaluation_results.timestamp_end =
			QLogicaeCore::TIME.now();
		result.set_to_good_status_with_value(
			file_system_evaluation_results
		);
	}

	std::future<AiseApiFileSystemEvaluationResults> AiseApi::evaluate_async(
		const AiseApiFileSystemEvaluationConfigurations& configurations
	)
	{
		std::promise<AiseApiFileSystemEvaluationResults> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, configurations,
			promise = std::move(promise)]() mutable
			{
				promise.set_value(
					evaluate(
						configurations
					)
				);
			}
		);

		return future;
	}

	void AiseApi::evaluate_async(
		QLogicaeCore::Result<std::future<AiseApiFileSystemEvaluationResults>>& result,
		const AiseApiFileSystemEvaluationConfigurations& configurations
	)
	{
		std::promise<AiseApiFileSystemEvaluationResults> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, configurations,
			promise = std::move(promise)]() mutable
			{				
				promise.set_value(
					evaluate(
						configurations
					)
				);
			}
		);

		result.set_to_good_status_with_value(
			std::move(future)
		);
	}

	void AiseApi::evaluate_async(
		const std::function<void(const AiseApiFileSystemEvaluationResults& result)>& callback,
		const AiseApiFileSystemEvaluationConfigurations& configurations
	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, configurations, callback]() mutable
			{
				callback(
					evaluate(
						configurations
					)
				);
			}
		);
	}

	void AiseApi::evaluate_async(
		const std::function<void(QLogicaeCore::Result<AiseApiFileSystemEvaluationResults>& result)>& callback,
		const AiseApiFileSystemEvaluationConfigurations& configurations
	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, configurations, callback]() mutable
			{
				QLogicaeCore::Result<AiseApiFileSystemEvaluationResults> result;

				evaluate(
					result,
					configurations
				);

				callback(
					result
				);
			}
		);
	}

	AiseApi& AiseApi::get_instance()
	{
		QLogicaeCore::Result<AiseApi*> result;

		get_instance(
			result
		);

		return *result.get_value();
	}

	void AiseApi::get_instance(
		QLogicaeCore::Result<AiseApi*>& result
	)
	{
		static AiseApi instance;

		result.set_to_good_status_with_value(
			&instance
		);
	}
}
