#pragma once

#include <tuple>

#include "typedefs-array.hpp"
#include "byref.hpp"

/// @brief Represents an exception thrown from usage of a Dictionary
struct DictionaryException : il2cpp_utils::exceptions::StackTraceException {
    void* dictionaryInstance;
    DictionaryException(void* instance, std::string_view msg) : il2cpp_utils::exceptions::StackTraceException(msg.data()), dictionaryInstance(instance) {}
};

template <typename TKey, typename TValue>
struct DictionaryEntry
{
	int32_t hashCode;
	int32_t next;
	TKey key;
	TValue value;

	template <int i>
    auto& get() {
        if constexpr (i == 0)
            return key;
        else if constexpr (i == 1)
            return value;
    }
};

// Allows decomposing to just key / value pairs
namespace std {
    template <class TKey, class TVal>
    struct tuple_size<DictionaryEntry<TKey, TVal>> : std::integral_constant<size_t, 2> {};

    template <class TKey, class TVal>
    struct tuple_element<0, DictionaryEntry<TKey, TVal>> {
        using type = TKey;
    };

    template <class TKey, class TVal>
    struct tuple_element<1, DictionaryEntry<TKey, TVal>> {
        using type = TVal;
    };
}

template <typename TKey, typename TValue>
struct Dictionary : Il2CppObject
{
	using Entry = DictionaryEntry<TKey, TValue>;
	
	ArrayW<int32_t> _buckets;
	ArrayW<Entry> _entries;
	int32_t _count;
	int32_t _freeList;
	int32_t _freeCount;
	int32_t _version;
	void* _comparer;
	Il2CppObject* _keys;
	Il2CppObject* _values;
	Il2CppObject* _syncRoot;

	int get_Count()
	{
		return _count - _freeCount;
	};

	TValue FindEntry(TKey key) {
        if (!static_cast<void*>(this)) throw DictionaryException(nullptr, "Running instance method on nullptr instance!");
        il2cpp_functions::Init();

        auto klass = il2cpp_functions::object_get_class(this);
        auto* ___internal_method = THROW_UNLESS(
            il2cpp_utils::FindMethod(
                klass,
                "FindEntry",
                std::array<Il2CppClass*, 0>{},
                ::std::array<const Il2CppType*, 1>{::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_type<TKey>::get()}
            )
        );
        return il2cpp_utils::RunMethodRethrow<TValue, false>(this, ___internal_method, key);
    }

	bool TryGetValue(TKey key, ::ByRef<TValue> value) {
        if (!static_cast<void*>(this)) throw DictionaryException(nullptr, "Running instance method on nullptr instance!");
        il2cpp_functions::Init();

        auto klass = il2cpp_functions::object_get_class(this);
        auto* ___internal_method = THROW_UNLESS(
            il2cpp_utils::FindMethod(
                klass,
                "TryGetValue",
                std::array<Il2CppClass*, 0>{},
                ::std::array<Il2CppType const*, 2>{ ::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_type<TKey>::get(), ::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_type<::ByRef<TValue>>::get() }
            )
        );
        return il2cpp_utils::RunMethodRethrow<bool, false>(this, ___internal_method, key, value);
    }

	bool TryInsert(TKey key, TValue value, uint8_t insertionBehavior) {
        if (!static_cast<void*>(this)) throw DictionaryException(nullptr, "Running instance method on nullptr instance!");
        il2cpp_functions::Init();

        auto klass = il2cpp_functions::object_get_class(this);
        auto* ___internal_method = THROW_UNLESS(
            il2cpp_utils::FindMethod(
                klass,
                "TryInsert",
                std::array<Il2CppClass*, 0>{},
                ::std::array<Il2CppType const*, 3>{ ::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_type<TKey>::get(), ::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_type<TValue>::get(), ::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_type<uint8_t>::get() }
            )
        );
        return il2cpp_utils::RunMethodRethrow<bool, false>(this, ___internal_method, key, value, insertionBehavior);
    }
};

template <class TKey, class TValue, class Ptr = Dictionary<TKey, TValue>*>
struct DictionaryWrapper
{
	constexpr DictionaryWrapper(Ptr dict): ptr(dict)
	{}

	template <class V = void>
        requires(std::is_pointer_v<Ptr> && !il2cpp_utils::has_il2cpp_conversion<Ptr>)
	constexpr DictionaryWrapper(void* alterInit): ptr(reinterpret_cast<Ptr>(alterInit))
	{}

	static constexpr uint8_t InsertionBehavior_None = static_cast<uint8_t>(0x0u);
	static constexpr uint8_t InsertionBehavior_OverwriteExisting = static_cast<uint8_t>(0x1u);

	using Entry = DictionaryEntry<TKey, TValue>;

	Ptr ptr;

	using pointer = Entry*;
	using const_pointer = const Entry*;
	
	using iterator = pointer;
	using const_iterator = const_pointer;

	iterator begin() {
		return reinterpret_cast<iterator>(&entries()[ptr->get_Count()]);
	}

	iterator end() {
		return reinterpret_cast<iterator>(&entries()[ptr->get_Count()]);
	}

	const_iterator begin() const {
		return entries().begin();
	}

	const_iterator end() const {
		return &entries()[ptr->get_Count()];
	}
	
	auto const& entries() const {
		return ptr->_entries;
	}

	auto& entries() {
		return ptr->_entries;
	}

	void insert(TKey key, TValue value) {
		ptr->TryInsert(key, value, InsertionBehavior_None);
	}

	void insert_or_assign(TKey key, TValue value) {
		ptr->TryInsert(key, value, InsertionBehavior_OverwriteExisting);
	}

	TValue get(TKey key) {
		TValue value;
		if(!ptr->TryGetValue(key, byref(value))) {
			throw DictionaryException(ptr, "Key not found!");
		}
		return value;
	}

	std::optional<TValue> try_get(TKey key) {
		TValue value;
		if(!ptr->TryGetValue(key, byref(value))){
			return std::nullopt;
		}
		return value;
	}

	TValue operator[](TKey key) {
        return get(key);
    }
    const TValue& operator[](TKey key) const {
        return get(key);
    }

	Ptr operator->() noexcept {
        return ptr;
    }

    Ptr const operator->() const noexcept {
        return ptr;
    }

	constexpr operator Ptr() {
		return ptr;
	}

	constexpr DictionaryWrapper<TKey, TValue>& operator=(Ptr const& ptr) {
        this->ptr = ptr;
        return *this;
    }
    constexpr DictionaryWrapper<TKey, TValue>& operator=(Ptr&& ptr) {
        this->ptr = ptr;
        return *this;
    }
};

DEFINE_IL2CPP_ARG_TYPE_GENERIC_CLASS(Dictionary, "System.Collections.Generic", "Dictionary`2");
DEFINE_IL2CPP_ARG_TYPE_GENERIC_STRUCT(DictionaryEntry, "System.Collections.Generic", "Dictionary`2/Entry");

#if !defined(NO_CODEGEN_WRAPPERS) && __has_include("System/Collections/Generic/Dictionary_2.hpp")
namespace System::Collections::Generic {
template <typename TKey, typename TValue>
class Dictionary_2;
}
// include header
#include "System/Collections/Generic/InsertionBehavior.hpp"
// forward declare usage
template <typename TKey, typename TValue, typename Ptr = System::Collections::Generic::Dictionary_2<TKey, TValue>*>
using DictionaryW = DictionaryWrapper<TKey, TValue, Ptr>;

#else
template <class TKey, class TValue, typename Ptr = Dictionary<TKey, TValue>*>
using DictionaryW = DictionaryWrapper<TKey, TValue, Ptr>;
#endif

template <typename TKey, typename TValue>
auto format_as(DictionaryEntry<TKey, TValue> dict) {
    if (!dict) return std::string("null");
    return fmt::format("{}: {}", dict.begin(), dict.end());
}

template <typename TKey, typename TValue, typename Ptr>
auto format_as(DictionaryWrapper<TKey, TValue, Ptr> dict) {
    if (!dict) return std::string("null");
    return fmt::join(dict.begin(), dict.end(), ", ");
}