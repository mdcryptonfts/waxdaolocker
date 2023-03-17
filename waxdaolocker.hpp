#pragma once
#define CONTRACT_NAME "waxdaolocker"

#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/symbol.hpp>
#include <string>
#include <eosio/crypto.hpp>
#include <eosio/transaction.hpp>
#include<map>
#include "constants.hpp"
#include "tables.hpp"
#define mix64to128(a, b) (uint128_t(a) << 64 | uint128_t(b))

using namespace eosio;


CONTRACT waxdaolocker : public contract {
	public:
		using contract::contract;

		waxdaolocker(name receiver, name code, datastream<const char *> ds):contract(receiver, code, ds){}

		//Main Actions
		ACTION cancellock(uint64_t lock_ID);
		ACTION createlock(name creator, name receiver, asset amount, name token_contract, uint64_t unlock_time);
		ACTION withdraw(uint64_t lock_ID);

		//Notifications
		[[eosio::on_notify("*::transfer")]] void receive_token_transfer(name from, name to, eosio::asset quantity, std::string memo);



	private:

		//Tables
		config_table config_t = config_table(get_self(), get_self().value);
		locks_table locks_t = locks_table(get_self(), get_self().value);

		//Functions
		void check_token_exists(symbol token_symbol, name contract);
		std::vector<std::string> get_words(std::string memo);
		uint64_t now();
		void transfer_tokens(name user, asset amount_to_send, name contract, std::string memo);





};
