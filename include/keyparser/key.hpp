#pragma once

#include "defines.hpp"

namespace keyparser {
	class Key {
	private:
		char s_data;
		std::string l_data;

		static Key getRoot();

	public:
		/// @brief Create short key
		/// @param s_data Short name [char]
		explicit Key(const char& s_data);

		/// @brief Create long key
		/// @param l_data Long name [string]
		explicit Key(const std::string& l_data);

		/// @brief Create full key
		/// @param s_data Short name [char]
		/// @param l_data Long name [string]
		explicit Key(const char& s_data, const std::string& l_data);

		/// @brief Compares two keys [current lower then given]
		/// @param key Key [Key&]
		bool operator<(const Key& key) const;

		/// @brief Compares two keys [full equivalent]
		/// @param key Key [Key&]
		bool operator==(const Key& key) const;

		/// @brief Compares two keys [not full equivalent]
		/// @param key Key [Key&]
		bool operator!=(const Key& key) const;

		/// @brief Compares two keys [partially equivalent]
		/// @param key Key [Key]
		bool operator^=(const Key& key) const;

		/// @brief Check if key is full
		bool full() const;

		/// @brief Return short name
		char sname() const;

		/// @brief Return long name
		std::string lname() const;

		/// @brief Return full name
		std::string fname() const;

		friend class Parser;
		friend class Task;
	};
}