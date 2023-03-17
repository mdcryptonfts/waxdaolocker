#pragma once

void waxdaolocker::receive_token_transfer(name from, name to, eosio::asset quantity, std::string memo) {

	const name tkcontract = get_first_receiver();

	if( from == get_self() || to != get_self() ) { return; }

	std::vector<std::string> words = get_words(memo);

	if(quantity.amount <= 0 || quantity.amount > MAX_SAFE_NUMBER){
		check(false, "Invalid quantity sent");
	}

	if(words[1] == "deposit"){

		const uint64_t lock_ID = std::strtoull(words[2].c_str(),NULL,0);

		auto itr = locks_t.require_find(lock_ID, "This lock ID does not exist");

		if(itr->status != 0){
			check(false, "This lock is either in progress or completed");
		}

		if(itr->token_contract != tkcontract){
			check(false, "Token contract does not match the listing");
		}

		if(itr->amount.symbol != quantity.symbol){
			check(false, "Token symbol does not match the listing");
		}

		if(quantity.amount < itr->amount.amount){
			check(false, "The entire lock balance must be depsited in 1 tx");
		}

		if(itr->unlock_time <= now()){
			check(false, "This lock has ended already");
		}

		locks_t.modify(itr, same_payer, [&](auto &_lock){
			_lock.status = 1;
			_lock.time_of_deposit = now();
		});
	}

}