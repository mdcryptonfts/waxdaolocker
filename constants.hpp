#pragma once

const uint64_t MAX_UNLOCK_TIME = 33260976000;
const int64_t MAX_ASSET_AMOUNT = 4611686018427387903;

const std::vector<uint8_t> LOCK_STATUSES = {
	0, //Created, awaiting deposit
	1, //Funded, in progress
	2, //Completed
};