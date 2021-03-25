void rotateSwapKey(uint8_t *cells8, uint16_t *cells16, uint64_t *cells64, uint8_t *key_block, uint8_t *swap_block, uint8_t *rotation_block){
	/*shift key block*/
	key_block[key_b_size - 1] = key_block[key_b_size - 1] ^ ciphered_txt_byte;

	shift_counter = 0;
	while(shift_counter < sub_shift_times){
		tmp_byte = key_block[subkey_shift_location] ^ rotation_block[rotation_counter_reset];
		rotation_counter_reset ++;
		if(rotation_counter_reset == rotation_b_size){
			rotation_counter_reset = 0;
		}
		for(uint64_t counter  = 0; counter < (key_b_size - 1); counter ++){
			key_block[counter] = key_block[counter + 1];
		}
		key_block[key_b_size - 1] = tmp_byte;
		shift_counter ++;
	}
	/*shift key block*/

	/*xor key 8 blocks of 32 bytes*/
	for(uint64_t counter = 0; counter < (4 * key_sections); counter ++){
		key_block[counter] = key_block[counter] ^ key_block[counter + (4 * key_sections)];
		key_block[counter + (4 * key_sections)] = key_block[counter + (4 * key_sections)] ^ key_block[counter + (8 * key_sections)];
		key_block[counter + (8 * key_sections)] = key_block[counter + (8 * key_sections)] ^ key_block[counter + (12 * key_sections)];
		key_block[counter + (12 * key_sections)] = key_block[counter + (12 * key_sections)] ^ key_block[counter + (16 * key_sections)];
		key_block[counter + (16 * key_sections)] = key_block[counter + (16 * key_sections)] ^ key_block[counter + (20 * key_sections)];
		key_block[counter + (20 * key_sections)] = key_block[counter + (20 * key_sections)] ^ key_block[counter + (24 * key_sections)];
		key_block[counter + (24 * key_sections)] = key_block[counter + (24 * key_sections)] ^ swap_block[counter + swap_section_counter];
	}
	swap_section_counter = swap_section_counter + (key_b_size / 8);
	if(swap_section_counter >= swap_b_size - (key_b_size / 8)){
		tmp_byte = swap_block[0];
		for(uint64_t ctr = 0; ctr < (swap_b_size - 1); ctr ++){
			swap_block[ctr + 1] = swap_block[ctr + 1] ^ swap_block[ctr];
			swap_block[ctr] = swap_block[ctr + 1];
		}
		swap_block[swap_b_size - 1] = tmp_byte;
		swap_section_counter = 0;
	}
	/*xor key 8 blocks of 32 bytes*/
}
