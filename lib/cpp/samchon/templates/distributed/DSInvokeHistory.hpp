#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/InvokeHistory.hpp>

namespace samchon
{
namespace templates
{
namespace distributed
{
	class DistributedSystem;
	class DistributedProcess;

	/**
	 * History of an {@link Invoke} message.
	 * 
	 * The {@link PRInvokeHistory} is a class archiving history log of an {@link Invoke} message which requests the
	 * *distributed process*, created whenever {@link DistributedProcess.sendData} is called.
	 * 
	 * When the *distributed process* has completed, then {@link complete complete()} is called and the *elapsed time* is 
	 * determined. The elapsed time is utilized for computation of {@link DistributedSystem.getPerformance performance index}
	 * and {@link DistributedProcess.getResource resource index} of related objects.
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_distributed_system.png)
	 *
	 * @handbook [Templates - Distributed System](https://github.com/samchon/framework/wiki/CPP-Templates-Distributed_System)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API DSInvokeHistory
		: public protocol::InvokeHistory
	{
		friend class DistributedSystem;

	private:
		typedef protocol::InvokeHistory super;

		DistributedSystem *system_;
		DistributedProcess *process_;
		double weight_;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Construct from a DistributedSystem.
		 * 
		 * @param system The {@link DistributedSystem} object who sent the {@link Invoke} message.
		 */
		DSInvokeHistory(DistributedSystem*);

		/**
		 * Initilizer Constructor.
		 * 
		 * @param system The {@link DistributedSystem} object who sent the {@link Invoke} message.
		 * @param process The {@link DistributedProcess} object who sent the {@link Invoke} message.
		 * @param invoke An {@link Invoke} message requesting the *distributed process*.
		 * @param weight Weight of resource which indicates how heavy this {@link Invoke} message is.
		 */
		DSInvokeHistory(DistributedSystem*, DistributedProcess*, std::shared_ptr<protocol::Invoke>, double weight);

		virtual ~DSInvokeHistory();

		virtual void construct(std::shared_ptr<library::XML>) override;

		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		 * Get the related {@link DistributedSystem} object.
		 */
		auto getSystem() const -> DistributedSystem*
		{
			return system_;
		};

		/**
		 * Get the related {@link DistributedProcess} object.
		 */
		auto getProcess() const -> DistributedProcess*
		{
			return process_;
		};

		/**
		 * Get weight.
		 * 
		 * Gets weight of resource which indicates how heavy this {@link Invoke} message is. Default is 1.
		 */
		auto getWeight() const -> double
		{
			return weight_;
		};

		/* ---------------------------------------------------------
			EXPORTERS
		--------------------------------------------------------- */
		virtual auto toXML() const -> std::shared_ptr<library::XML> override;
	};
};
};
};