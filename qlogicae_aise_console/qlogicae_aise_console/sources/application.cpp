#include "pch.hpp"

#include "../includes/application.hpp"

namespace QLogicaeAiseConsole
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	bool Application::setup(
		int argc,
		char** argv
	)
	{
		QLogicaeCore::Result<void> void_result;

		try
		{
			setup(
				void_result,
				argc,
				argv
			);

			return void_result.is_status_safe();
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::setup()",
				exception.what()
			);

			return false;
		}
	}

	std::future<bool> Application::setup_async(
		int argc,
		char** argv
	)
	{
		std::promise<bool> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, argc, argv,
			promise = std::move(promise)]() mutable
			{
				promise.set_value(
					setup(
						argc,
						argv
					)
				);
			}
		);

		return future;
	}

	void Application::setup_async(
		QLogicaeCore::Result<std::future<void>>& result,
		int argc,
		char** argv
	)
	{
		std::promise<void> promise;
		auto future = promise.get_future();

		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, argc, argv,
			promise = std::move(promise)]() mutable
			{
				QLogicaeCore::Result<void> result;

				setup(
					result,
					argc,
					argv
				);

				promise.set_value();
			}
		);

		result.set_to_good_status_with_value(
			std::move(future)
		);
	}

	void Application::setup(
		QLogicaeCore::Result<void>& result,
		int argc,
		char** argv
	)
	{
		QLogicaeCore::LogConfigurations console_log_configurations_1 =
		{
			.is_console_enabled = false,
			.is_console_format_enabled = true
		};

		QLogicaeCore::QLOGICAE_APPLICATION.setup(result);
		if (result.is_status_unsafe())
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::setup()",
				"Setup failed"
			);

			return result.set_to_bad_status_without_value();
		}

		UTILITIES.setup(result);
		if (result.is_status_unsafe())
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::setup()",
				"Setup failed"
			);

			return result.set_to_bad_status_without_value();
		}

		UTILITIES.CLI_APPLICATION.name(
			UTILITIES.get_application_full_name()
		);

		LOGGER.setup(result);
		if (result.is_status_unsafe())
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::setup()",
				"Setup failed"
			);

			return result.set_to_bad_status_without_value();
		}

		QLogicaeAiseCore::AISE_API.setup(
			result,
			QLogicaeAiseCore::AiseApiConfigurations
			{}
		);
		if (result.is_status_unsafe())
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::setup()",
				"Setup failed"
			);

			return result.set_to_bad_status_without_value();
		}

		QLogicaeCore::Result<bool> boolean_result;
		QLogicaeAiseCore::ENCODING_MANAGER.setup(
			result
		);
		QLogicaeAiseCore::ENCODING_MANAGER.load_vocabulary_file_content(
			boolean_result,
			QLogicaeCore::UTILITIES.FULL_EXECUTABLE_FOLDER_PATH + "/qlogicae/application/ai/encodings/vocabulary.json",
			97
		);
		if (result.is_status_unsafe() ||
			boolean_result.is_status_unsafe())
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::setup()",
				"Setup failed"
			);

			return result.set_to_bad_status_without_value();
		}

		QLogicaeAiseCore::NeuralNetworkModel::get_instance().setup(
			result,
			QLogicaeCore::UTILITIES.FULL_EXECUTABLE_FOLDER_PATH + "/qlogicae/application/ai/models/model.onnx"
		);
		if (result.is_status_unsafe())
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::setup()",
				"Setup failed"
			);

			return result.set_to_bad_status_without_value();
		}
		
		if (
			!_setup_view_command() ||
			!_setup_evaluate_command()
			)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::setup()",
				"Parsing Failed"
			);

			return result.set_to_bad_status_without_value();
		}

		try
		{
			UTILITIES.CLI_APPLICATION.parse(argc, argv);
		}
		catch (const CLI::CallForHelp& exception)
		{
			QLogicaeCore::Result<std::future<void>> future_void_result;

			QLogicaeCore::LOGGER.log_with_timestamp_to_files_async(
				future_void_result,
				"Help has been asked"
			);

			std::exit(UTILITIES.CLI_APPLICATION.exit(exception));

			return result.set_to_bad_status_without_value();
		}
		catch (const CLI::ParseError& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::setup()",
				exception.what()
			);

			std::exit(UTILITIES.CLI_APPLICATION.exit(exception));

			return result.set_to_bad_status_without_value();
		}

		return result.set_to_good_status_without_value();
	}

	std::future<bool> Application::setup_async(
		const std::function<void(const bool& result)>& callback,
		int argc,
		char** argv
	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, argc, argv, callback]() mutable
			{
				callback(
					setup(
						argc,
						argv
					)
				);
			}
		);
	}

	void Application::setup_async(
		const std::function<void(QLogicaeCore::Result<void>& result)>& callback,
		int argc,
		char** argv

	)
	{
		boost::asio::post(
			QLogicaeCore::UTILITIES.BOOST_ASIO_POOL,
			[this, argc, argv, callback]() mutable
			{
				QLogicaeCore::Result<void> result;

				setup(
					result,
					argc,
					argv
				);

				callback(
					result
				);
			}
		);
	}

	bool Application::terminate()
	{
		QLogicaeCore::Result<void> void_result;

		try
		{
			terminate(void_result);

			return void_result.is_status_safe();
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::terminate()",
				exception.what()
			);

			return false;
		}
	}

	std::future<bool> Application::terminate_async()
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

	void Application::terminate_async(
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

	void Application::terminate(
		QLogicaeCore::Result<void>& result
	)
	{
		QLogicaeCore::Result<void> void_result;

		QLogicaeCore::QLOGICAE_APPLICATION.terminate(result);
		if (result.is_status_unsafe())
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::terminate()",
				"Termination failed"
			);

			return result.set_to_bad_status_without_value();
		}

		UTILITIES.terminate(result);
		if (result.is_status_unsafe())
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::terminate()",
				"Termination failed"
			);

			return result.set_to_bad_status_without_value();
		}

		result.set_to_good_status_without_value();
	}

	std::future<bool> Application::terminate_async(
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

	void Application::terminate_async(
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

	bool Application::parse()
	{
		QLogicaeCore::Result<void> void_result;

		try
		{
			for (const auto& [key, value] : UTILITIES.CLI_APPLICATION_COMMANDS)
			{
				if (value.first->parsed())
				{
					return value.second();
				}
			}

			return true;
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::parse()",
				exception.what()
			);

			return false;
		}
	}

	Application& Application::get_instance()
	{
		QLogicaeCore::Result<Application*> result;

		get_instance(
			result
		);

		return *result.get_value();
	}

	void Application::get_instance(
		QLogicaeCore::Result<Application*>& result
	)
	{
		static Application instance;

		result.set_to_good_status_with_value(
			&instance
		);
	}

	bool Application::_setup_view_command()
	{
		try
		{
			CLI::App* view_command =
				UTILITIES.CLI_APPLICATION.add_subcommand(
					"view",
					"about"
				);
			view_command->alias("vw");

			CLI::App* view_about_command =
				view_command->add_subcommand(
					"about",
					"displays application details"
				);
			view_about_command->alias("a");

			view_about_command
				->add_option("--is-verbose",
					BOOLEAN_INPUTS.get("view_about", "is_verbose"),
					"enables or disables verbose console logging")
				->default_val(false);

			UTILITIES.CLI_APPLICATION_COMMANDS["view_about"] = std::make_pair(
				view_about_command,
				[this]() -> bool
				{
					QLogicaeCore::Result<void> void_result;

					bool view_about_command__is_verbose =
						BOOLEAN_INPUTS.get(
							"view_about", "is_verbose"
						);

					QLogicaeCore::LogConfigurations console_log_configurations_1 =
					{
						.is_console_enabled = view_about_command__is_verbose,
						.is_console_format_enabled = view_about_command__is_verbose
					};

					QLogicaeCore::LogConfigurations console_log_configurations_2 =
					{
						.is_console_enabled = true,
						.is_console_format_enabled = view_about_command__is_verbose
					};

					try
					{
						LOGGER.log_running(
							void_result,
							"aise view about",
							console_log_configurations_1
						);

						LOGGER.log(
							void_result,
							UTILITIES.get_application_about_details(),
							console_log_configurations_2
						);

						LOGGER.log_complete(
							void_result,
							"aise view about",
							console_log_configurations_1
						);

						return true;
					}
					catch (const std::exception& exception)
					{
						QLogicaeCore::LOGGER.handle_exception_async(
							"QLogicaeAiseConsole::Application::_setup_view_command()",
							exception.what()
						);

						return false;
					}
				}
			);

			return true;
		}
		catch (const std::exception& exception)
		{

			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::_setup_view_command()",
				exception.what()
			);

			return false;
		}
	}

	bool Application::_setup_evaluate_command()
	{
		try
		{
			CLI::App* evaluate_command =
				UTILITIES.CLI_APPLICATION.add_subcommand(
					"evaluate",
					"evaluate .hpp and .cpp files within the current executed path"
				);
			evaluate_command->alias("e");
			
			evaluate_command
				->add_option("--target-folder-path",
					STRING_INPUTS.get("evaluate", "path"),
					"root folder path to evaluate files recursively")
				->default_val(QLogicaeCore::UTILITIES.FULL_EXECUTED_FOLDER_PATH);

			evaluate_command
				->add_option("--target-extensions",
					STRING_INPUTS.get("evaluate", "extensions"),
					"expected file extensions for parsing and evaluation")
				->default_val(".hpp,.cpp");

			evaluate_command
				->add_option("--minimum-positive-prediction",
					DOUBLE_INPUTS.get("evaluate", "minimum_positive_prediction"),
					"the minimum prediction inclusive value to be labeled as 'positive'")
				->check(CLI::Range(0.0, 1.0))
				->default_val(0.99);

			evaluate_command
				->add_option("--maximum-positive-prediction",
					DOUBLE_INPUTS.get("evaluate", "maximum_positive_prediction"),
					"the maximum prediction inclusive value to be labeled as 'positive'")
				->check(CLI::Range(0.0, 1.0))
				->default_val(1);

			evaluate_command
				->add_option("--is-overview-visible",
					BOOLEAN_INPUTS.get("evaluate", "is_overview_visible"),
					"enables or disables evaluation overview information display")
				->default_val(true);

			evaluate_command
				->add_option("--is-positive-line-prediction-visible",
					BOOLEAN_INPUTS.get("evaluate", "is_positive_line_prediction_visible"),
					"enables or disables positive line prediction information display")
				->default_val(true);

			evaluate_command
				->add_option("--is-evaluation-file-output-enabled",
					BOOLEAN_INPUTS.get("evaluate", "is_evaluation_file_output_enabled"),
					"enables or disables evaluation file output")
				->default_val(true);

			evaluate_command
				->add_option("--evaluation-folder-output-path",
					STRING_INPUTS.get("evaluate", "evaluation_folder_output_path"),
					"the relative path of the evaluation folder output")
				->default_val("qlogicae\\.qlogicae\\aise\\evaluate");

			evaluate_command
				->add_option("--evaluation-file",
					STRING_INPUTS.get("evaluate", "evaluation_file"),
					"the name and extensions of the evaluation file output itself")
				->default_val(
					QLogicaeCore::TIME.now(
						QLogicaeCore::TimeFormat::FULL_DASHED_TIMESTAMP
					) + ".log"
				);

			evaluate_command
				->add_option("--is-verbose",
					BOOLEAN_INPUTS.get("evaluate", "is_verbose"),
					"enables or disables verbose console logging")
				->default_val(false);

			UTILITIES.CLI_APPLICATION_COMMANDS["evaluate"] = std::make_pair(
				evaluate_command,
				[this]() -> bool
				{					
					bool
						evaluate_command__is_verbose,
						evaluate_command__is_overview_visible,
						evaluate_command__is_evaluation_file_output_enabled,
						evaluate_command__is_positive_line_prediction_visible;

					size_t
						total_line_count,
						total_files_found_count,
						total_files_parsed_count,
						total_folders_found_count,
						total_positive_prediction_count,
						total_negative_prediction_count;

					double
						total_timestamp_end,
						total_timestamp_start,
						total_positive_prediction_count_ratio,
						total_negative_prediction_count_ratio,
						total_positive_prediction_count_percentage,
						total_negative_prediction_count_percentage,
						evaluate_command__minimum_positive_prediction,
						evaluate_command__maximum_positive_prediction;

					std::string
						token,
						evaluate_command__path,
						evaluate_command__extensions,
						evaluate_command__evaluation_file,
						evaluate_command__evaluation_folder_output_path;


					std::vector<std::string>
						evaluate_command__expected_file_extensions_vector;

					QLogicaeCore::Result<void>
						void_result;

					QLogicaeCore::Result<QLogicaeAiseCore::AiseApiFileSystemEvaluationResults>
						aise_results;

					tabulate::Table
						summary_table;

					QLogicaeCore::TextFileIO
						text_file_io;

					std::ostringstream
						output_stream;

					DWORD
						mode;

					HANDLE
						handle;

					CONSOLE_CURSOR_INFO
						cursor_info;

					evaluate_command__path =
						STRING_INPUTS.get(
							"evaluate", "path"
						);

					evaluate_command__extensions =
						STRING_INPUTS.get(
							"evaluate", "extensions"
						);

					evaluate_command__minimum_positive_prediction =
						DOUBLE_INPUTS.get(
							"evaluate", "minimum_positive_prediction"
						);

					evaluate_command__maximum_positive_prediction =
						DOUBLE_INPUTS.get(
							"evaluate", "maximum_positive_prediction"
						);

					evaluate_command__is_overview_visible =
						BOOLEAN_INPUTS.get(
							"evaluate", "is_overview_visible"
						);

					evaluate_command__is_positive_line_prediction_visible =
						BOOLEAN_INPUTS.get(
							"evaluate", "is_positive_line_prediction_visible"
						);

					evaluate_command__extensions =
						STRING_INPUTS.get(
							"evaluate", "extensions"
						);

					evaluate_command__is_evaluation_file_output_enabled =
						BOOLEAN_INPUTS.get(
							"evaluate", "is_evaluation_file_output_enabled"
						);

					evaluate_command__evaluation_folder_output_path =
						STRING_INPUTS.get(
							"evaluate", "evaluation_folder_output_path"
						);

					evaluate_command__evaluation_file =
						STRING_INPUTS.get(
							"evaluate", "evaluation_file"
						);

					evaluate_command__is_verbose =
						BOOLEAN_INPUTS.get(
							"evaluate", "is_verbose"
						); 

					for (char character : evaluate_command__extensions)
					{
						if (character == ',')
						{
							evaluate_command__expected_file_extensions_vector
								.push_back(token);
							token.clear();
						}
						else
						{
							token.push_back(
								character
							);
						}
					}
					evaluate_command__expected_file_extensions_vector
						.push_back(token);

					QLogicaeCore::LogConfigurations console_log_configurations_1 =
					{
						.is_console_enabled = evaluate_command__is_verbose,
						.is_console_format_enabled = evaluate_command__is_verbose
					};

					QLogicaeCore::LogConfigurations console_log_configurations_2 =
					{
						.is_console_enabled = true,
						.is_console_format_enabled = evaluate_command__is_verbose
					};

					try
					{
						SetConsoleOutputCP(CP_UTF8);
						SetConsoleCP(CP_UTF8);
						handle = GetStdHandle(STD_OUTPUT_HANDLE);
						mode = 0;						
						GetConsoleMode(handle, &mode);
						mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
						SetConsoleMode(handle, mode);
						GetConsoleCursorInfo(handle, &cursor_info);
						cursor_info.bVisible = FALSE;
						SetConsoleCursorInfo(handle, &cursor_info);

						std::cout << "\x1b[2J\x1b[H";

						indicators::ProgressSpinner progress_spinner
						{
							indicators::option::PostfixText{""},
							indicators::option::ForegroundColor{indicators::Color::white},
							indicators::option::SpinnerStates{
								std::vector<std::string>{"⠈","⠐","⠠","⢀","⡀","⠄","⠂","⠁"}
							},
							indicators::option::FontStyles{
								std::vector<indicators::FontStyle>{indicators::FontStyle::bold}
							}
						};

						progress_spinner.set_option(
							indicators::option::ForegroundColor
							{
								indicators::Color::white
							}
						);

						progress_spinner.set_option(
							indicators::option::ShowPercentage
							{
								false
							}
						);

						QLogicaeAiseCore::AISE_API.evaluate(
							aise_results,
							QLogicaeAiseCore::AiseApiFileSystemEvaluationConfigurations
							{
								.root_folder_path =
									evaluate_command__path,
								.expected_file_extensions =
									evaluate_command__expected_file_extensions_vector,
								.minimum_positive_prediction =
									evaluate_command__minimum_positive_prediction,
								.maximum_positive_prediction =
									evaluate_command__maximum_positive_prediction,
								.file_evaluation_callback =
									[&progress_spinner](const QLogicaeAiseCore::AiseApiFileSystemEvaluationCallbackConfigurationsResults& callback_results)
								{
									progress_spinner.set_option(
										indicators::option::PostfixText
										{
											callback_results.text
										}
									);

									std::cout << "\x1b[2J\x1b[H";
									progress_spinner.tick();
								}
							}
						);

						progress_spinner.set_option(
							indicators::option::PostfixText
							{
								"  Generating Summary"
							}
						);

						QLogicaeAiseCore::AiseApiFileSystemEvaluationResults& aise_results_pointer =
							aise_results.get_value();

						total_line_count =
							aise_results_pointer.total_line_count;
						total_positive_prediction_count =
							aise_results_pointer.total_positive_prediction_count;
						total_negative_prediction_count =
							total_line_count - total_positive_prediction_count;

						total_timestamp_start =
							aise_results_pointer.timestamp_start;
						total_timestamp_end =
							aise_results_pointer.timestamp_end;

						total_folders_found_count =
							aise_results_pointer.total_folders_found_count;
						total_files_found_count =
							aise_results_pointer.total_files_found_count;
						total_files_parsed_count =
							aise_results_pointer.file_evaluation_results.size();

						total_positive_prediction_count_ratio =
							(total_line_count) ?
							(static_cast<double>(total_positive_prediction_count) / static_cast<double>(total_line_count)) :
							0;

						total_positive_prediction_count_percentage =
							(total_positive_prediction_count_ratio) * 100;

						total_negative_prediction_count_ratio =
							(total_line_count) ?
							static_cast<double>(total_negative_prediction_count) / static_cast<double>(total_line_count) :
							0;
						total_negative_prediction_count_percentage =
							(total_negative_prediction_count_ratio) * 100;

						summary_table.add_row(
							{
								"Property",
								"Value"
							}
						);

						summary_table.add_row(
							{
								"Application Version",
								QLogicaeCore::QLOGICAE_APPLICATION_UTILITIES.CONFIGURATIONS_APPLICATION_VERSION
							}
						);

						summary_table.add_row(
							{
								"Timestamp Created",
								QLogicaeCore::TIME.now(
									QLogicaeCore::TimeFormat::ISO8601
								)
							}
						);

						summary_table.add_row(
							{
								"Evaluation Duration (In Seconds)",
								std::to_string(
									(total_timestamp_end - total_timestamp_start) / 1'000'000'000
								)
							}
						);

						summary_table.add_row(
							{
								"Is File Output Enabled",
								(evaluate_command__is_evaluation_file_output_enabled) ?
									"true" :
									"false"
							}
						);

						summary_table.add_row(
							{
								"Target Folder Path",
								(evaluate_command__is_evaluation_file_output_enabled) ?
									evaluate_command__path :
									"None"
							}
						);
						
						summary_table.add_row(
							{
								"File Output Path",
								(evaluate_command__is_evaluation_file_output_enabled) ?
									evaluate_command__evaluation_folder_output_path +
									"\\" + evaluate_command__evaluation_file :
									"None"
								
							}
						);

						summary_table.add_row(
							{
								"Target Extensions",
								(evaluate_command__extensions.size()) ? evaluate_command__extensions : "all"
							}
						);

						summary_table.add_row(
							{
								"Minimum Positive Line Prediction",
								std::to_string(
									evaluate_command__minimum_positive_prediction
								)
							}
						);

						summary_table.add_row(
							{
								"Maximum Positive Line Prediction",
								std::to_string(
									evaluate_command__maximum_positive_prediction
								)
							}
						);


						summary_table.add_row(
							{
								"Folders Found",
								std::to_string(
									total_folders_found_count
								)
							}
						);

						summary_table.add_row(
							{
								"Files Found",
								std::to_string(
									total_files_found_count
								)
							}
						);

						summary_table.add_row(
							{
								"Files Parsed",
								std::to_string(
									total_files_parsed_count
								)
							}
						);

						summary_table.add_row(
							{
								"Line Count",
								std::to_string(
									total_line_count
								)
							}
						);

						summary_table.add_row(
							{
								"Positive Line Predictions",
								std::to_string(
									total_positive_prediction_count
								) + " | " +
								std::to_string(
									total_positive_prediction_count_percentage
								) + "% | " +
								std::to_string(
									total_positive_prediction_count_ratio
								)
							}
						);

						summary_table.add_row(
							{
								"Negative Line Predictions",
								std::to_string(
									total_negative_prediction_count
								) + " | " +
								std::to_string(
									total_negative_prediction_count_percentage
								) + "% | " +
								std::to_string(
									total_negative_prediction_count_ratio
								)
							}
						);

						summary_table.format()
							.font_style({ tabulate::FontStyle::bold })
							.border_top("-")
							.border_bottom("-")
							.border_left("|")
							.border_right("|")
							.corner("+");

						if (evaluate_command__is_overview_visible)
						{
							output_stream << summary_table;
						}

						if (evaluate_command__is_positive_line_prediction_visible &&
							aise_results_pointer.total_positive_prediction_count
						)
						{
							for (const auto& [file_path, file_evaluation_result] :
								aise_results_pointer.positive_file_evaluation_results
							)
							{
								tabulate::Table file_table;

								file_table.add_row(
									{
										"Prediction",
										"Line",
										"Text"
									}
								);

								output_stream
									<< "\n\n> "
									<< "\x1b]8;;file:///" << file_path << "\x1b\\"
									<< file_path
									<< "\x1b]8;;\x1b\\"
									<< "\n";

								for (const auto& file_line_evaluation_result :
									file_evaluation_result.file_line_evaluation_results)
								{
									file_table.add_row(
										{
											std::to_string(
												file_line_evaluation_result.line_prediction
											),
											std::to_string(
												file_line_evaluation_result.line_number
											),
											file_line_evaluation_result.line_text
										}
									);
								}

								file_table.format()
									.font_align(tabulate::FontAlign::left)
									.border_top("-")
									.border_bottom("-")
									.border_left("|")
									.border_right("|")
									.corner("+");

								output_stream << file_table;

								std::cout << "\x1b[2J\x1b[H";
								progress_spinner.tick();
							}
						}

						output_stream << "\n";
						
						std::cout << "\x1b[2J\x1b[H";

						progress_spinner.set_option(
							indicators::option::PostfixText
							{
								"✔  Evaluation Complete"
							}
						);

						progress_spinner.mark_as_completed();						

						LOGGER.log_running(
							void_result,
							"aise evaluate",
							console_log_configurations_1
						);
						
						LOGGER.log(
							void_result,
							std::string("\n") + output_stream.str() + std::string("\n"),
							console_log_configurations_2
						);

						if (evaluate_command__is_evaluation_file_output_enabled)
						{
							if (!std::filesystem::exists(evaluate_command__evaluation_folder_output_path))
							{
								std::filesystem::create_directories(
									evaluate_command__evaluation_folder_output_path
								);															
							}

							text_file_io.setup(
								evaluate_command__evaluation_folder_output_path +
								"\\" + evaluate_command__evaluation_file
							);

							text_file_io.write(
								output_stream.str()
							);
						}

						LOGGER.log_complete(
							void_result,
							"aise evaluate",
							console_log_configurations_1
						);

						handle = GetStdHandle(STD_OUTPUT_HANDLE);
						GetConsoleCursorInfo(handle, &cursor_info);
						cursor_info.bVisible = TRUE;
						SetConsoleCursorInfo(handle, &cursor_info);

						return true;
					}
					catch (const std::exception& exception)
					{
						QLogicaeCore::LOGGER.handle_exception_async(
							"QLogicaeAiseConsole::Application::_setup_evaluate_command()",
							exception.what()
						);

						return false;
					}
				}
			);

			return true;
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::Application::_setup_evaluate_command()",
				exception.what()
			);

			return false;
		}
	}
}
