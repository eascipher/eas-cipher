void rotateTranspoKey(uint8_t *cells8, uint16_t *cells16, uint64_t *cells64, uint8_t *transpotition_block, uint8_t *transpo_swap_block, uint8_t *rotation_block){
	/*shift key block*/
	transpotition_block[transpotition_b_size - 1] = transpotition_block[transpotition_b_size - 1] ^ ciphered_txt_byte;

	shift_counter = 0;
	while(shift_counter < tra_shift_times){
		tmp_byte = transpotition_block[trakey_shift_location] ^ rotation_block[rotation_counter_reset];
		rotation_counter_reset ++;
		if(rotation_counter_reset == rotation_b_size){
			rotation_counter_reset = 0;
		}
		for(uint64_t counter  = 0; counter < (transpotition_b_size - 1); counter ++){
			transpotition_block[counter] = transpotition_block[counter + 1];
		}
		transpotition_block[transpotition_b_size - 1] = tmp_byte;
		shift_counter ++;
	}
	/*shift key block*/

	/*xor key 8 blocks of 32 bytes*/
	for(uint64_t counter = 0; counter < (4 * transpo_sections); counter ++){
		transpotition_block[counter] = transpotition_block[counter] ^ transpotition_block[counter + (4 * transpo_sections)];
		transpotition_block[counter + (4 * transpo_sections)] = transpotition_block[counter + (4 * transpo_sections)] ^ transpotition_block[counter + (8 * transpo_sections)];
		transpotition_block[counter + (8 * transpo_sections)] = transpotition_block[counter + (8 * transpo_sections)] ^ transpotition_block[counter + (12 * transpo_sections)];
		transpotition_block[counter + (12 * transpo_sections)] = transpotition_block[counter + (12 * transpo_sections)] ^ transpotition_block[counter + (16 * transpo_sections)];
		transpotition_block[counter + (16 * transpo_sections)] = transpotition_block[counter + (16 * transpo_sections)] ^ transpotition_block[counter + (20 * transpo_sections)];
		transpotition_block[counter + (20 * transpo_sections)] = transpotition_block[counter + (20 * transpo_sections)] ^ transpotition_block[counter + (24 * transpo_sections)];
		transpotition_block[counter + (24 * transpo_sections)] = transpotition_block[counter + (24 * transpo_sections)] ^ transpo_swap_block[counter + swap_section_counter];
	}
	swap_section_counter = swap_section_counter + (transpotition_b_size / 8);
	if(swap_section_counter >= tswap_b_size - (transpotition_b_size / 8)){
		tmp_byte = transpo_swap_block[0];
		for(uint64_t ctr = 0; ctr < (tswap_b_size - 1); ctr ++){
			transpo_swap_block[ctr + 1] = transpo_swap_block[ctr + 1] ^ transpo_swap_block[ctr];
			transpo_swap_block[ctr] = transpo_swap_block[ctr + 1];
		}
		transpo_swap_block[(tswap_b_size - 1)] = tmp_byte;
		swap_section_counter = 0;
	}
	/*xor key 8 blocks of 32 bytes*/
}
