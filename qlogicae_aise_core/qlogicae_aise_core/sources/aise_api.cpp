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
		AiseApiFileSystemEvaluationResults file_system_evaluation_results;

		std::cout << configurations.relative_folder_path << "\n";
		std::cout << QLogicaeCore::UTILITIES.FULL_EXECUTED_FOLDER_PATH +
			"\\" + configurations.relative_folder_path << "\n";

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
