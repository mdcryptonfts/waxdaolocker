#pragma once

//Tables not specific to this contract

struct [[eosio::table]] stat {
	eosio::asset    	supply;
	eosio::asset 		max_supply;
	eosio::name 		issuer;

	uint64_t primary_key()const { return supply.symbol.code().raw(); }
};
typedef eosio::multi_index< "stat"_n, stat > stat_table;

//Tables specific to this contract

struct [[eosio::table, eosio::contract(CONTRACT_NAME)]] config {
	uint64_t 					ID;
	uint64_t 					last_lock;

	uint64_t primary_key() const { return ID; }
};
using config_table = eosio::multi_index<"config"_n, config
>;

struct [[eosio::table, eosio::contract(CONTRACT_NAME)]] locks {
	uint64_t 		ID;
	eosio::name 	creator;
	eosio::name  	receiver;
	eosio::asset 	amount;
	eosio::name 	token_contract;
	uint64_t 		time_of_creation;
	uint64_t  		time_of_deposit;
	uint64_t 		unlock_time;
	uint8_t 		status;

	uint64_t primary_key() const { return ID; }
	uint64_t second_key() const { return creator.value; }
	uint64_t third_key() const { return receiver.value; }
};
using locks_table = eosio::multi_index<"locks"_n, locks,
eosio::indexed_by<"creator"_n, eosio::const_mem_fun<locks, uint64_t, &locks::second_key>>,
eosio::indexed_by<"receiver"_n, eosio::const_mem_fun<locks, uint64_t, &locks::third_key>>
>;