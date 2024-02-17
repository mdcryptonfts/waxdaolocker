#pragma once

#include "waxdaolocker.hpp"
#include "functions.cpp"
#include "on_notify.cpp"

ACTION waxdaolocker::cancellock(uint64_t lock_ID)
{
	auto itr = locks_t.require_find(lock_ID, "This lock ID does not exist");

	if(itr->status != 0){
		check(false, "This lock is not able to be cancelled");
	}

	require_auth(itr->creator);

	itr = locks_t.erase(itr);
}

ACTION waxdaolocker::createlock(name creator, name receiver, asset amount, name token_contract, uint64_t unlock_time)
{

	require_auth(creator);

	auto itr = config_t.require_find(0, "Could not locate config");
	uint64_t next_lock = itr->last_lock + 1;

	config_t.modify(itr, same_payer, [&](auto &_config){
		_config.last_lock = next_lock;
	});

	if(!is_account(receiver)){
		check(false, "Receiving account does not exist");
	}

	if(amount.amount <= 0){
		check(false, "Amount must be positive");
	}

	if(amount.amount > MAX_ASSET_AMOUNT){
		check(false, "Amount is too large");
	}

	check_token_exists(amount.symbol, token_contract);

	if(unlock_time <= now() + 86400){
		check(false, "Unlock time must be at least 1 day from now");
	}

	if(unlock_time > MAX_UNLOCK_TIME){
		check(false, "Invalid unlock time");
	}

	locks_t.emplace(creator, [&](auto &_lock){
		_lock.ID = next_lock;
		_lock.creator = creator;
		_lock.receiver = receiver;
		_lock.amount = amount;
		_lock.token_contract = token_contract;
		_lock.time_of_creation = now();
		_lock.time_of_deposit = 1111111111;
		_lock.unlock_time = unlock_time;
		_lock.status = 0;
	});

}

ACTION waxdaolocker::withdraw(uint64_t lock_ID)
{
	auto itr = locks_t.require_find(lock_ID, "This lock ID does not exist");

	if(itr->status != 1){
		check(false, "This lock is either completed, cancelled or not funded");
	}

	if(itr->unlock_time > now()){
		check(false, "Lock has not ended yet");
	}

	transfer_tokens(itr->receiver, itr->amount, itr->token_contract, "Your waxdaolocker withdrawal has been processed");

	locks_t.modify(itr, same_payer, [&](auto &_lock){
		_lock.status = 2;
	});
}