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
			{

			}
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
					"evaluations"
				);
			evaluate_command->alias("e");
			
			evaluate_command
				->add_option("--relative-folder-path",
					STRING_INPUTS.get("evaluate", "relative_folder_path"),
					"Relative folder path to evaluate")
				->default_val("");


			evaluate_command
				->add_option("--is-verbose",
					BOOLEAN_INPUTS.get("evaluate", "is_verbose"),
					"Enables or disables verbose console logging")
				->default_val(false);

			UTILITIES.CLI_APPLICATION_COMMANDS["evaluate"] = std::make_pair(
				evaluate_command,
				[this]() -> bool
				{
					QLogicaeCore::Result<void> void_result;

					bool evaluate_command__is_verbose =
						BOOLEAN_INPUTS.get(
							"evaluate", "is_verbose"
						);
					std::string evaluate_command__relative_folder_path =
						STRING_INPUTS.get(
							"evaluate", "relative_folder_path"
						);

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
						LOGGER.log_running(
							void_result,
							"aise evaluate",
							console_log_configurations_1
						);

						QLogicaeCore::Result<QLogicaeAiseCore::AiseApiFileSystemEvaluationResults> aise_results;

						QLogicaeAiseCore::AISE_API.evaluate(
							aise_results,
							QLogicaeAiseCore::AiseApiFileSystemEvaluationConfigurations
							{
								.relative_folder_path = evaluate_command__relative_folder_path
							}
							);

						LOGGER.log_complete(
							void_result,
							"aise evaluate",
							console_log_configurations_1
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
