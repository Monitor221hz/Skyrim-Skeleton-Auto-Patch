#pragma once

#include "RE/H/hkArray.h"
#include "RE/H/hkStringPtr.h"

namespace RE
{
	class hkClass;

	/// This is the root level class exported by our tools.
	/// It contains an array of Variants and associated names.
	class hkRootLevelContainer
	{
	public:
		/// A variant with a name string.
		/// This class also redundantly stores the class name so that you can identify
		/// the type even if this object comes from a packfile without metadata (i.e., with
		/// null m_variant.m_class)
		class NamedVariant
		{
		public:
			// members
			hkStringPtr  name;       // 00
			hkStringPtr  className;  // 08
			hkRefVariant variant;    // 10
		};
		static_assert(sizeof(NamedVariant) == 0x18);

		static const hkClass& staticClass() { return *REL::Relocation<hkClass*>(REL::ID(521941)); }

		void* findObjectByType(const char* typeName, void* prevObject = nullptr) const
		{
			using func_t = decltype(&hkRootLevelContainer::findObjectByType);
			REL::Relocation<func_t> func{ RELOCATION_ID(59481, 60157) };
			return func(this, typeName, prevObject);
		}

		// members
		hkArray<NamedVariant> namedVariants;  // 00
	};
	static_assert(sizeof(hkRootLevelContainer) == 0x10);
}