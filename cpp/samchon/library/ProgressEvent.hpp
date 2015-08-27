#pragma once
#include <samchon/library/Event.hpp>

namespace samchon
{
	namespace library
	{
		/** 
		 * @brief Event for expressing progress
		 * @details
		 *
		 * @author Jeongho Nam
		 */
		class SAMCHON_FRAMEWORK_API ProgressEvent
			: public Event
		{
		public:
			enum TYPES : int
			{
				PROGRESS = 11
			};

		protected:
			/**
			 * @brief The number of current progress
			 */
			double numerator;

			/**
			 * @brief The number of total progress
			 * @warning denominator cannot be zero
			 */
			double denominator;

		public:
			/**
			 * @brief Construct from source and progress
			 * @details 
			 *
			 * @param source Source of the event; who made the event
			 * @param numerator The number of current progress
			 * @param denominator The number of total progress
			 *
			 * @throw exception denominator is zero
			 */
			ProgressEvent(EventDispatcher *, double, double);
			virtual ~ProgressEvent() = default;

			auto getNumerator() const -> double;
			auto getDenominator() const -> double;

			/**
			 * @brief Get percentage of prgress
			 * @details 
			 */
			auto getPercent() const -> double;
		};
	};
};