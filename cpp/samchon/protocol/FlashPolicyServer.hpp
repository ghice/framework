#pragma once
#include <samchon/API.hpp>

#include <memory>
#include <samchon/protocol/Socket.hpp>

namespace samchon
{
	namespace library
	{
		class XML;
	};
	namespace protocol
	{
		/**
		 * @brief A policy server for flash
		 *
		 * @details 
		 * FlashPolicyServer is a policy server 
		 * which is replying cross-domain-policy to flash client.
		 */
		class SAMCHON_FRAMEWORK_API FlashPolicyServer
		{
		protected:
			/**
			 * @brief cross-domain-policy string
			 */
			std::string policy;

		public:
			/**
			 * @brief Default constructor
			 *
			 * @details The cross-domain-policy is to accept all, any client.
			 */
			FlashPolicyServer();

			/**
			 * @brief Construct from custom policy
			 *
			 * @param policy Custom cross-domain-policy to adjust
			 */
			FlashPolicyServer(std::shared_ptr<library::XML> policy);

			/**
			 * @brief Open server
			 */
			void openServer();

		private:
			/**
			 * @brief Accept client
			 *
			 * @details Accepts flash client and replies cross-domain-policy
			 */
			void accept(Socket *);
		};
	};
};