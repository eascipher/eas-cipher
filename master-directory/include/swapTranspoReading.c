void swapTranspoReading(uint8_t *cells8, uint16_t *cells16, uint64_t *cells64, uint8_t *lock_block, uint8_t** rotor, uint8_t** rev_rotor, uint8_t** transpo, uint8_t** rev_transpo){
	/*load rotors*/
	rotor_counter = 0;
	lock_rotor_counter = 32;
	while(rotor_counter < 256){
		for(uint16_t counter = 0; counter < 256; counter ++){
			rotor[rotor_counter][counter] = lock_block[lock_rotor_counter];
			lock_rotor_counter ++;
		}
		rotor_counter ++;
	}
	/*load rotors*/

	/*verify rotors*/
	verify_errors = 0;
	duplicate_errors = 0;

	for(uint16_t pcounter = 0; pcounter < 256; pcounter ++){
		for(uint16_t counter = 0; counter < 256; counter ++){
			verify_notfound = 0;
			while(verify_notfound < 256){
				if(rotor[pcounter][verify_notfound] != counter){
					verify_notfound ++;
					if(verify_notfound == 256){
						verify_errors ++;
					}
					continue;
				}
				break;
			}
		}
	}
	/*verify rotors*/

	/*serial block*/
	uint64_t *serial_block;
	serial_block = (uint64_t*)malloc(256 * sizeof(uint64_t));
	/*serial block*/

	/*serial numbers*/
	for(uint16_t rcounter = 0; rcounter < 256; rcounter ++){
		check_less = 64000;
		for(uint16_t counter = 0; counter < 256; counter = counter + 2){
			check_less = check_less + rotor[rcounter][counter];
			check_less = check_less - rotor[rcounter][counter + 1];
		}
		checksum_multi = 0;
		for(uint16_t counter = 0; counter < 255; counter ++){
			checksum_multi = checksum_multi + (rotor[rcounter][counter] * rotor[rcounter][counter + 1]);
		}
		checksum = 0;
		reverse_packet_counter = 255;
		for(uint16_t counter = 0; counter < 128; counter ++){
			checksum = checksum + (rotor[rcounter][counter] * rotor[rcounter][reverse_packet_counter]);
			reverse_packet_counter --;
		}
		serial_block[rcounter] = checksum * checksum_multi * check_less;
	}
	/*serial numbers*/

	/*checks duplicate rotors*/
	tracking_counter = 1;
	while(tracking_counter < 256){
		for(uint16_t counter = 0; counter < tracking_counter; counter ++){
			if(serial_block[tracking_counter] == serial_block[counter]){
				duplicate_errors ++;
			}
		}
		tracking_counter ++;
	}
	/*checks duplicate rotors*/

	/*print serial numbers*/
	if(print_serial == 1 && duplicate_errors == 0 && verify_errors == 0){
		for(uint16_t counter = 0; counter < 256; counter ++){
			printf("rotor: %"PRIu16"\t#: %"PRIu64"\n", counter, serial_block[counter]);
		}
	}
	/*print serial numbers*/

	/*generate reverse rotors*/
	rotor_counter = 0;
	while(rotor_counter < 256){
	        for(uint16_t counter = 0; counter < 256; counter ++){
	                search_byte = rotor[rotor_counter][counter];
	                rev_rotor[rotor_counter][search_byte] = counter;
	        }
	        rotor_counter ++;
	}
	/*generate reverse rotors*/

	/*load transpotition tables*/
	transpo_counter = 0;
	lock_transpo_counter = 32 + 65536;
	while(transpo_counter < 256){
		for(uint16_t counter = 0; counter < 256; counter ++){
			transpo[transpo_counter][counter] = lock_block[lock_transpo_counter];
			lock_transpo_counter ++;
		}
		transpo_counter ++;
	}
	/*load transpotition tables*/

	/*verify transpotition tables*/
	for(uint16_t pcounter = 0; pcounter < 256; pcounter ++){
		for(uint16_t counter = 0; counter < 256; counter ++){
			verify_notfound = 0;
			while(verify_notfound < 256){
				if(transpo[pcounter][verify_notfound] != counter){
					verify_notfound ++;
					if(verify_notfound == 256){
						verify_errors ++;
					}
					continue;
				}
				break;
			}
		}
	}
	/*verify transpotition tables*/

	/*serial numbers*/
	for(uint16_t rcounter = 0; rcounter < 256; rcounter ++){
		check_less = 64000;
		for(uint16_t counter = 0; counter < 256; counter = counter + 2){
			check_less = check_less + transpo[rcounter][counter];
			check_less = check_less - transpo[rcounter][counter + 1];
		}
		checksum_multi = 0;
		for(uint16_t counter = 0; counter < 255; counter ++){
			checksum_multi = checksum_multi + (transpo[rcounter][counter] * transpo[rcounter][counter + 1]);
		}
		checksum = 0;
		reverse_packet_counter = 255;
		for(uint16_t counter = 0; counter < 128; counter ++){
			checksum = checksum + (transpo[rcounter][counter] * transpo[rcounter][reverse_packet_counter]);
			reverse_packet_counter --;
		}
		serial_block[rcounter] = checksum * checksum_multi * check_less;
	}
	/*serial numbers*/

	/*checks duplicate tables*/
	tracking_counter = 1;
	while(tracking_counter < 256){
		for(uint16_t counter = 0; counter < tracking_counter; counter ++){
			if(serial_block[tracking_counter] == serial_block[counter]){
				duplicate_errors ++;
			}
		}
		tracking_counter ++;
	}
	/*checks duplicate tables*/

	/*print serial numbers*/
	if(print_serial == 1 && duplicate_errors == 0 && verify_errors == 0){
		for(uint16_t counter = 0; counter < 256; counter ++){
			printf("table: %"PRIu16"\t#: %"PRIu64"\n", counter, serial_block[counter]);
		}
	}
	free(serial_block);
	/*print serial numbers*/

	/*generate reverse transpotition tables*/
	transpo_counter = 0;
	while(transpo_counter < 256){
	        for(uint16_t counter = 0; counter < 256; counter ++){
	                search_byte = transpo[transpo_counter][counter];
	                rev_transpo[transpo_counter][search_byte] = counter;
	        }
	        transpo_counter ++;
	}
	/*generate reverse transpotition tables*/
}
