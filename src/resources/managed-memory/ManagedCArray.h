#pragma once
#include <resources/ManagedMemoryRegistry.h>

namespace resources {
	template<typename T, typename S>
	class ManagedCArray {
		ManagedAllocator allocator;
		T*& arr;
		S& length;

	public:
		ManagedCArray(hh::fnd::ManagedResource* resource, T*& arr, S& length) : allocator{ ManagedMemoryRegistry::instance->GetManagedAllocator(resource) }, arr { arr }, length{ length } {}

		void push_back(const T& item) {
			auto* newBuffer = new (std::align_val_t(alignof(T)), &allocator) T[length + 1]{};
			
			for (S i = 0; i < length; i++)
				newBuffer[i] = std::move(arr[i]);

			newBuffer[length] = item;

			allocator.Free(arr);
			arr = newBuffer;

			length++;
		}

		void push_back(T&& item) {
			auto* newBuffer = new (std::align_val_t(alignof(T)), &allocator) T[length + 1]{};

			for (S i = 0; i < length; i++)
				newBuffer[i] = std::move(arr[i]);

			newBuffer[length] = std::move(item);

			allocator.Free(arr);
			arr = newBuffer;

			length++;
		}

		T& emplace_back() {
			auto* newBuffer = new (std::align_val_t(alignof(T)), &allocator) T[length + 1]{};

			for (S i = 0; i < length; i++)
				newBuffer[i] = std::move(arr[i]);

			allocator.Free(arr);
			arr = newBuffer;

			length++;

			return arr[length - 1];
		}

		T& emplace(S i) {
			emplace_back();

			for (S j = length - 1; j >= i + 1; j--)
				arr[j] = std::move(arr[j - 1]);

			return arr[i];
		}

		void remove(S i) {
			arr[i].~T();

			for (S j = i + 1; j < length; j++)
				arr[j - 1] = std::move(arr[j]);

			length--;
		}

		void clear() {
			for (S i = 0; i < length; i++)
				arr[i].~T();

			length = 0;
		}

		bool empty() const { return length == 0; }
		const T& operator[](S i) const { return arr[i]; }
		T& operator[](S i) { return arr[i]; }
		T* begin() { return &arr[0]; }
		T* end() { return &arr[length]; }
		S size() const { return length; }
		S capacity() const { return length; }
	};
}
