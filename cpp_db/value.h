#ifndef CPP_DB_VALUE_H
#define CPP_DB_VALUE_H

#include "null.h"
#include "type_of.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace cpp_db
{
	class value
	{
	public:
		value() = default;

		template<typename T>
        value(T val)
            : pholder(new concrete_holder<T>(val))
		{
		}

        value(const char *val)
            : pholder(new concrete_holder<std::string>(val))
        {
        }

        value(char *val)
            : pholder(new concrete_holder<std::string>(val))
        {
        }

        value(const wchar_t *val)
            : pholder(new concrete_holder<std::wstring>(val))
        {
        }

        value(wchar_t *val)
            : pholder(new concrete_holder<std::wstring>(val))
        {
        }

		value(const value  &other)
			: pholder(other.pholder->clone())
		{
		}

		value &operator=(const value &other)
		{
			if (this != &other)
				pholder.reset(other.pholder->clone());
			return *this;
		}

        value(value &&) = default;
        value &operator=(value &&) = default;

		template<typename T>
		T get_value() const
		{
			if (pholder == nullptr)
				throw std::runtime_error("Invalid value object");

            if (pholder->get_type() == typeid(T))
				return *reinterpret_cast<T const *>(pholder->get_value());
            throw std::runtime_error(std::string("Invalid value type (")+typeid(T).name() + std::string(" / ") + pholder->get_type().name()+")");
		}

		template<typename T, typename U>
		U cast_to() const
		{
			if (pholder == nullptr)
				throw std::runtime_error("Invalid value object");

			if (pholder->get_type() == typeid(T))
                return static_cast<concrete_holder<T> *>(pholder.get())->template cast_to<U>();
			throw std::runtime_error(std::string("Invalid value type (") + +typeid(T).name() + std::string(" != ") + pholder->get_type().name() + ")");
		}

		template<typename T>
		bool has_value_of_type() const
		{
			if (pholder == nullptr)
				throw std::runtime_error("Invalid value object");
			return pholder->get_type() == typeid(T);
		}

		friend std::type_index type_of(const value &v);
		friend bool is_null(const value &v);

	private:
		struct abstract_holder
		{
			virtual ~abstract_holder() {}
			virtual void const * get_value() const = 0;
			virtual std::type_index get_type() const = 0;
			virtual abstract_holder *clone() const = 0;
		};

        template<typename ValueType>
        struct concrete_holder : public abstract_holder
        {
            explicit concrete_holder(ValueType value_in)
                : value(value_in)
            {
            }

            void const * get_value() const override
            {
                return &value;
            }

			std::type_index get_type() const override
			{
				return typeid(ValueType);
			}

			concrete_holder *clone() const override
			{
				return new concrete_holder(value);
			}

			template<typename CastType>
			CastType cast_to() const
			{
				return value;
			}

			ValueType value;
		};

		std::unique_ptr<abstract_holder> pholder;
	};

	inline std::type_index type_of(const value &v)
	{
		return v.pholder->get_type();
	}

	inline bool is_null(const value &v)
	{
		return v.pholder->get_type() == typeid(null_type);
	}

}

#include "value.inl"

#endif
