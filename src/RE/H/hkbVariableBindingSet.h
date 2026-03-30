#pragma once 
#include "RE/H/hkMemoryRouter.h"
namespace RE
{
    class hkbVariableBindingSet : public hkReferencedObject
	{
	public:
		inline static constexpr auto RTTI = RTTI_hkbVariableBindingSet;
		inline static constexpr auto VTABLE = VTABLE_hkbVariableBindingSet;

		struct Binding
		{
		public:
			/// Which data we are binding to.
			enum class BindingType : uint8_t
			{
				/// Binding to a variable.
				BINDING_TYPE_VARIABLE,

				/// Binding to a character property.
				BINDING_TYPE_CHARACTER_PROPERTY,
			};

			enum class Flags : uint8_t
			{
				FLAG_NONE = 0,

				// values are copied from the property to bound variables
				FLAG_OUTPUT = 1,
			};

			Binding() = default;
			Binding(const char* memberPath, int32_t variableIndex, BindingType bindingType, int8_t bitIndex);

			bool isOutput() const { return flags.all(Flags::FLAG_OUTPUT); };

			// members
			hkStringPtr                      memberPath;                                         // 00
			hkClass*                         memberClass{ nullptr };                             // 08
			int32_t                          offsetInObjectPlusOne{ 0 };                         // 10
			int32_t                          offsetInArrayPlusOne{ 0 };                          // 14
			int32_t                          rootVariableIndex{ -1 };                            // 18
			int32_t                          variableIndex{ 0 };                                 // 1C
			int8_t                           bitIndex{ -1 };                                     // 20
			BindingType                      bindingType{ BindingType::BINDING_TYPE_VARIABLE };  // 21
			int8_t                           memberType{ 0 };                                    // 22
			int8_t                           variableType{ -1 };                                 // 23
			stl::enumeration<Flags, uint8_t> flags{};                                            // 24
			char                             pad25[3];                                           // 25
		};
		static_assert(sizeof(Binding) == 0x28);

		hkbVariableBindingSet() { stl::emplace_vtable(this); }
		~hkbVariableBindingSet() override = default;  // 00

		HK_HEAP_REDEFINE_NEW();

		// members
		hkArray<Binding> bindings;                      // 10
		int32_t          indexOfBindingToEnable{ -1 };  // 20
		bool             hasOutputBinding{ false };     // 24
		char             pad25[3];                      // 25
	};
	static_assert(sizeof(hkbVariableBindingSet) == 0x28);
}