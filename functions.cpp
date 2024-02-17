#pragma once

void waxdaolocker::check_token_exists( symbol token_symbol, name contract ){
	const uint64_t raw_token_symbol = token_symbol.code().raw();
	const uint128_t symbol_contract_combo = mix64to128(raw_token_symbol, contract.value);
	stat_table stat(contract, token_symbol.code().raw());
	auto stat_itr = stat.find(token_symbol.code().raw());
	check(stat_itr != stat.end(), "That token does not exist on that contract");
	check(stat_itr->supply.symbol == token_symbol, "Symbol mismatch. You probably put the wrong amount of decimals in the precision field");
	return;
}

std::vector<std::string> waxdaolocker::get_words(std::string memo){
	std::string delim = "|";
	std::vector<std::string> words{};
	size_t pos = 0;
	while ((pos = memo.find(delim)) != std::string::npos) {
		words.push_back(memo.substr(0, pos));
		memo.erase(0, pos + delim.length());
	}
	return words;
}

uint64_t waxdaolocker::now() {
  return current_time_point().sec_since_epoch();
}

void waxdaolocker::transfer_tokens(name user, asset amount_to_send, name contract, std::string memo){

	action(permission_level{get_self(), "active"_n}, contract,"transfer"_n,std::tuple{ get_self(), user, amount_to_send, memo}).send();

	return;

}