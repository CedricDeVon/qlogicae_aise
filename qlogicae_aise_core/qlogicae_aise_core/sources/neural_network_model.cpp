#include "pch.hpp"

#include "../includes/neural_network_model.hpp"

namespace QLogicaeAiseConsole
{
	NeuralNetworkModel::NeuralNetworkModel()
	{
		setup();
	}

	NeuralNetworkModel::~NeuralNetworkModel()
	{
		terminate();
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
				"QLogicaeAiseConsole::NeuralNetworkModel::setup()",
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

	std::future<bool> NeuralNetworkModel::setup_async(
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
				"QLogicaeAiseConsole::NeuralNetworkModel::terminate()",
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

	std::future<bool> NeuralNetworkModel::terminate_async(
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
		const std::vector<float>& input
	)
	{
		try
		{
			QLogicaeCore::Result<void> result;

			setup(result);

			return result.get_value();
		}
		catch (const std::exception& exception)
		{
			QLogicaeCore::LOGGER.handle_exception_async(
				"QLogicaeAiseConsole::NeuralNetworkModel::predict()",
				exception.what()
			);

			return 0.0;
		}
	}

	void NeuralNetworkModel::predict(
		QLogicaeCore::Result<double>& result,
		const std::vector<float>& input
	)
	{

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
