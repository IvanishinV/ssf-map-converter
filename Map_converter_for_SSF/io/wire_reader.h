#pragma once

#include "util/endian.h"

#include <cstddef>
#include <cstring>
#include <span>
#include <stdexcept>
#include <type_traits>

class WireReader
{
public:
	explicit WireReader(std::span<const std::byte> buf) noexcept
		: m_buf(buf), m_offset(0) {}

	template <typename T>
	[[nodiscard]] T read()
	{
		static_assert(std::is_trivially_copyable_v<T>);
		check(sizeof(T));
		T value;
		std::memcpy(&value, m_buf.data() + m_offset, sizeof(T));
		m_offset += sizeof(T);
		return value;
	}

	template <typename T>
	[[nodiscard]] T peek_at(std::size_t offset) const
	{
		static_assert(std::is_trivially_copyable_v<T>);
		if (offset + sizeof(T) > m_buf.size())
			throw std::out_of_range("WireReader::peek_at");
		T value;
		std::memcpy(&value, m_buf.data() + offset, sizeof(T));
		return value;
	}

	[[nodiscard]] std::span<const std::byte> read_span(std::size_t n)
	{
		check(n);
		auto s = m_buf.subspan(m_offset, n);
		m_offset += n;
		return s;
	}

	void skip(std::size_t n) { check(n); m_offset += n; }
	void seek(std::size_t p) { if (p > m_buf.size()) throw std::out_of_range("WireReader::seek"); m_offset = p; }

	[[nodiscard]] std::size_t tell()      const noexcept { return m_offset; }
	[[nodiscard]] std::size_t remaining() const noexcept { return m_buf.size() - m_offset; }
	[[nodiscard]] bool        eof()       const noexcept { return m_offset >= m_buf.size(); }

private:
	void check(std::size_t need) const
	{
		if (m_offset + need > m_buf.size())
			throw std::out_of_range("WireReader past end");
	}

	std::span<const std::byte> m_buf;
	std::size_t m_offset;
};
