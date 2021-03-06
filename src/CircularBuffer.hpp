// https://github.com/rlogiacco/CircularBuffer

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

namespace Helper {
	template<bool FITS8, bool FITS16> struct Index {
		using Type = uint32_t;
	};

	template<> struct Index<false, true> {
		using Type = uint16_t;
	};

	template<> struct Index<true, true> {
		using Type = uint8_t;
	};
}

template<typename T, size_t S, typename IT = typename Helper::Index<(S <= UINT8_MAX), (S <= UINT16_MAX)>::Type> class CircularBuffer {
public:
	/**
	 * The buffer capacity: read only as it cannot ever change.
	 */
	static constexpr IT capacity = static_cast<IT>(S);

	/**
	 * Aliases the index type, can be used to obtain the right index type with `decltype(buffer)::index_t`.
	 */
	using index_t = IT;

	constexpr CircularBuffer();

	/**
	 * Disables copy constructor
	 */
	CircularBuffer(const CircularBuffer&) = delete;
	CircularBuffer(CircularBuffer&&) = delete;

	/**
	 * Disables assignment operator
	 */
	CircularBuffer& operator=(const CircularBuffer&) = delete;
	CircularBuffer& operator=(CircularBuffer&&) = delete;

	/**
	 * Adds an element to the beginning of buffer: the operation returns `false` if the addition caused overwriting an existing element.
	 */
	bool unshift(T value);

	/**
	 * Adds an element to the end of buffer: the operation returns `false` if the addition caused overwriting an existing element.
	 */
	bool push(T value);

	/**
	 * Removes an element from the beginning of the buffer.
	 */
	T shift();

	/**
	 * Removes an element from the end of the buffer.
	 */
	T pop();

	/**
	 * Returns the element at the beginning of the buffer.
	 */
	T inline first() const;

	/**
	 * Returns the element at the end of the buffer.
	 */
	T inline last() const;

	/**
	 * Array-like access to buffer
	 */
	T operator [] (IT index) const;

	/**
	 * Returns how many elements are actually stored in the buffer.
	 */
	IT inline size() const;

	/**
	 * Returns how many elements can be safely pushed into the buffer.
	 */
	IT inline available() const;

	/**
	 * Returns `true` if no elements can be removed from the buffer.
	 */
	bool inline isEmpty() const;

	/**
	 * Returns `true` if no elements can be added to the buffer without overwriting existing elements.
	 */
	bool inline isFull() const;

	/**
	 * Resets the buffer to a clean status, making all buffer positions available.
	 */
	void inline clear();
    
private:
	T buffer[S];
	T *head;
	T *tail;
#ifndef CIRCULAR_BUFFER_INT_SAFE
	IT count;
#else
	volatile IT count;
#endif
};

// CircularBuffer template implementation

template<typename T, size_t S, typename IT>
constexpr CircularBuffer<T,S,IT>::CircularBuffer() :
		head(buffer), tail(buffer), count(0) {
}

template<typename T, size_t S, typename IT>
bool CircularBuffer<T,S,IT>::unshift(T value) {
	if (head == buffer) {
		head = buffer + capacity;
	}
	*--head = value;
	if (count == capacity) {
		if (tail-- == buffer) {
			tail = buffer + capacity - 1;
		}
		return false;
	} else {
		if (count++ == 0) {
			tail = head;
		}
		return true;
	}
}

template<typename T, size_t S, typename IT>
bool CircularBuffer<T,S,IT>::push(T value) {
	if (++tail == buffer + capacity) {
		tail = buffer;
	}
	*tail = value;
	if (count == capacity) {
		if (++head == buffer + capacity) {
			head = buffer;
		}
		return false;
	} else {
		if (count++ == 0) {
			head = tail;
		}
		return true;
	}
}

template<typename T, size_t S, typename IT>
T CircularBuffer<T,S,IT>::shift() {
	if (count <= 0) abort();
	T result = *head++;
	if (head >= buffer + capacity) {
		head = buffer;
	}
	count--;
	return result;
}

template<typename T, size_t S, typename IT>
T CircularBuffer<T,S,IT>::pop() {
	if (count <= 0) abort();
	T result = *tail--;
	if (tail < buffer) {
		tail = buffer + capacity - 1;
	}
	count--;
	return result;
}

template<typename T, size_t S, typename IT>
T inline CircularBuffer<T,S,IT>::first() const {
	return *head;
}

template<typename T, size_t S, typename IT>
T inline CircularBuffer<T,S,IT>::last() const {
	return *tail;
}

template<typename T, size_t S, typename IT>
T CircularBuffer<T,S,IT>::operator [](IT index) const {
	return *(buffer + ((head - buffer + index) % capacity));
}

template<typename T, size_t S, typename IT>
IT inline CircularBuffer<T,S,IT>::size() const {
	return count;
}

template<typename T, size_t S, typename IT>
IT inline CircularBuffer<T,S,IT>::available() const {
	return capacity - count;
}

template<typename T, size_t S, typename IT>
bool inline CircularBuffer<T,S,IT>::isEmpty() const {
	return count == 0;
}

template<typename T, size_t S, typename IT>
bool inline CircularBuffer<T,S,IT>::isFull() const {
	return count == capacity;
}

template<typename T, size_t S, typename IT>
void inline CircularBuffer<T,S,IT>::clear() {
	head = tail = buffer;
	count = 0;
}

#endif //CircularBuffer.hpp