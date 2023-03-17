#pragma once

const uint64_t MAX_SAFE_NUMBER = 18446744073709551615;

const std::vector<uint8_t> LOCK_STATUSES = {
	0, //Created, awaiting deposit
	1, //Funded, in progress
	2, //Completed
};