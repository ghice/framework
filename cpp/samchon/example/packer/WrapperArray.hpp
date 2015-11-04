#pragma once
#include <samchon/protocol/SharedEntityArray.hpp>
#include <samchon/example/packer/Wrapper.hpp>

#include <samchon/library/FactorialGenerator.hpp>

#include <iostream>

namespace samchon
{
	namespace example
	{
		namespace packer
		{
			/**
			 * @brief An array of wrapper with same category (name).
			 *
			 * @details
			 * <p> WrapperArray reserves Product(s) and generates enough number of Wrapper(s). </p>
			 * <p> WrapperArray will retrieve the best arrange and packaging method for reserved Product(s). </p> 
			 *
			 * <p> @image html cpp/example_packer.png
			 * @image latex cpp/example_packer.png </p>
			 *
			 * @author Jeongho Nam
			 */
			class WrapperArray
				: public protocol::SharedEntityArray<Wrapper>
			{
			private:
				typedef protocol::SharedEntityArray<Wrapper> super;

			protected:
				/**
				 * @brief A list for reserved Product(s).
				 */
				std::shared_ptr<ProductArray> reserved;

				/**
				 * @brief A sample wrapper used to copy
				 */
				std::shared_ptr<Wrapper> sample;

			public:
				/* ---------------------------------------------------------
					CONSTRUCTORS
				--------------------------------------------------------- */
				/**
				 * @brief Default Constructor
				 */
				WrapperArray()
					: super()
				{
					this->reserved = std::make_shared<ProductArray>();
					this->sample = std::make_shared<Wrapper>();
				};

				/**
				 * @brief Construct from arguments of sample.
				 * 
				 * @param name Category name of a wrapper, can be identified
				 * @param price Price of a wrapper
				 * @param volume Limited volume of a wrapper can put in.
				 * @param weight Limited weight of a wrapper can put in.
				 */
				WrapperArray(const std::string &name, int price, int volume, int weight)
					: super()
				{
					this->reserved = std::make_shared<ProductArray>();
					this->sample = std::make_shared<Wrapper>(name, price, volume, weight);
				};


				/**
				* @brief Copy Constructor
				*
				* @details 
				* <p> Copy constructor of WrapperArray does not copy children items. </p> 
				* <p> Only copies sample. </p>
				*/
				WrapperArray(const WrapperArray &wrapperArray)
					: super()
				{
					this->reserved = std::make_shared<ProductArray>();
					this->sample = wrapperArray.sample;
				};

				virtual ~WrapperArray() = default;

				virtual void construct(std::shared_ptr<library::XML> xml) override
				{
					super::construct(xml);

					sample->construct(xml);
				};

			protected:
				virtual auto createChild(std::shared_ptr<library::XML>) -> Wrapper* override
				{
					return new Wrapper();
				};

			public:
				/**
				 * @brief Try to insert a product into reserved list.
				 *
				 * @details 
				 * If the Product's volume and weight is equal or less than the Wrapper categorized so that enable to
				 * insert in a Wrapper, reserve the Product and returns <i>true</i>. If not, does not reserve and just
				 * return <i>false</i>.
				 *
				 * @return Whether the Product's volume and weight is equal or less than the Wrapper.
				 */
				auto tryInsert(std::shared_ptr<Product> product) -> bool
				{
					if (product->getVolume() > sample->getVolume() ||
						product->getWeight() > sample->getWeight())
					{
						return false;
					}

					reserved->push_back(product);
					return true;
				};

				/* ---------------------------------------------------------
					OPERATORS
				--------------------------------------------------------- */
				/**
				 * @brief Optimize to retrieve the best solution.
				 *
				 * @details
				 * <p> Retrieve the best solution of packaging in level of WrapperArray. </p>
				 * <p> Shuffles sequence of reserved Product(s) by samchon::library::FactorialGenerator and insert the reserved
				 * Products(s) following the sequence creating Wrapper(s) as needed. Between the sequences from FactorialGenerator,
				 * retrieve and determine the best solution. </p>
				 *
				 * @note 
				 * <p> Sequence of inserting Product can affeact to numbers of Wrapper(s) to be used. </p>
				 * <p> It's the reason why even WrapperArray has the optimize() method. </p>
				 *
				 * @see samchon::library::FactorialGenerator
				 */
				void optimize()
				{
					if(reserved->empty() == true)
						return;

					library::FactorialGenerator factorial(reserved->size());
					std::shared_ptr<WrapperArray> minWrapperArray = nullptr;

					for (size_t i = 0; i < factorial.size(); i++)
					{
						std::shared_ptr<WrapperArray> wrapperArray(new WrapperArray(*this));
						std::vector<size_t> &row = factorial[i];

						for (size_t j = 0; j < row.size(); j++)
						{
							std::shared_ptr<Product> &product = this->reserved->at(row[j]);

							if (wrapperArray->empty() == true ||
								wrapperArray->at(wrapperArray->size() - 1)->tryInsert(product) == false)
							{
								Wrapper *wrapper = new Wrapper(*this->sample);
								wrapper->tryInsert(product);

								wrapperArray->emplace_back(wrapper);
							}
						}

						//unique_lock<mutex> uk(mtx);
						if (minWrapperArray == nullptr ||
							wrapperArray->size() < minWrapperArray->size())
						{
							minWrapperArray = wrapperArray;
						}
					}

					assign(minWrapperArray->begin(), minWrapperArray->end());
				};

				/**
				 * @brief Calculate price of the Wrapper(s)
				 * @details Calculates price of all wrapppers'. The price does not contain inserted products'
				 */
				auto calcPrice() const -> int
				{
					return sample->getPrice() * (int)size();
				};

				/* ---------------------------------------------------------
					EXPORT
				--------------------------------------------------------- */
				virtual auto TAG() const -> std::string override
				{
					return "wrapperArray";
				}
				virtual auto CHILD_TAG() const -> std::string override
				{
					return "wrapper";
				};

				virtual auto toXML() const -> std::shared_ptr<library::XML> override
				{
					std::shared_ptr<library::XML> &xml = super::toXML();
					xml->addAllProperty(sample->toXML());

					return xml;
				};

				/**
				 * @brief Return a string represents Wrapper(s) of same type
				 */
				auto toString() const -> std::string
				{
					std::string str = "Category - " + sample->getName() + "\n";
					for (size_t i = 0; i < size(); i++)
						str += at(i)->toString() + "\n";

					return move(str);
				};
			};
		};
	};
};