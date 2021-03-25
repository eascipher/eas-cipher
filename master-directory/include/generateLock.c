void generateLock(uint8_t *cells8, uint16_t *cells16, uint64_t *cells64, char **strg, char **cargv){
	struct timeval tiempo;

        ////////////////////////////////////*generates lock*////////////////////////////////////////
	FILE* infile;
	infile = fopen(gen_lock_file_name, output_mode);
	if(infile == NULL){
		printf("couldn't open %s.\n", gen_lock_file_name);
		exit(22);
	}

	/*initstate array*/
	static char random_array[256];
	initstate(0xFFFFFFFF, random_array, sizeof(random_array));
	/*initstate array*/

	uint64_t amount_of_rot_tab = 256;

	rotor_b_size = (256 * amount_of_rot_tab);
	transpotition_b_size = (256 * amount_of_rot_tab);
	keysize = 32 * kbsize;
	transpotitionsize = keysize;
	transpotitionsize_o = transpotitionsize;
	swapsize = keysize * 8;
	rotationsize = keysize * 8;
	keysize_o = keysize;
	swapsize_o = swapsize;
	rotationsize_o = rotationsize;


	whole_size = (32 + keysize_o + swapsize_o + rotationsize_o + rotor_b_size + transpotition_b_size + transpotitionsize_o);

	/*check size*/
	if(lock_required == 1){
		fseek(infile, 0, SEEK_END);
		drive_size = ftell(infile);
		if(whole_size > drive_size - 2){
			printf("cipher-lock-256bit size is %"PRIu64" bytes.  %s size is %"PRIu64" bytes.\n",whole_size, gen_lock_file_name , drive_size - 2);
			fclose(infile);
			exit(90);
		}
		if(drive_size < whole_size + lock_address){
			printf("cipher-lock-256bit boundary error. max start address is %"PRIu64".  %s size is %"PRIu64" bytes.\n", drive_size - whole_size, gen_lock_file_name , drive_size);
			fclose(infile);
			exit(90);
		}
	}
	/*check size*/

	/******************cipher code**********************/
	uint8_t *cipher_cb = (uint8_t*)malloc(256 * sizeof(uint8_t));
	uint64_t cipher_code_drive_size, cipher_code_reset = 0;
	if(cipher_code == 1){
		FILE *cipher_code_file;
		cipher_code_file = fopen(cipher_code_file_name, "rb");
		if(cipher_code_file == NULL){
			printf("couldn't open %s.\n", cipher_code_file_name);
			fclose(infile);
			exit(200);
		}
		if(cipher_code_required == 0){
			fseek(cipher_code_file, 0, SEEK_END);
			cipher_code_size = ftell(cipher_code_file);
			if(cipher_code_size < 300 || cipher_code_size > 500000){
				printf("%s is %"PRIu64" bytes.  -cc must be 300 to 500,000 bytes long.\n", cipher_code_file_name, cipher_code_size);
				fclose(infile);
				fclose(cipher_code_file);
				exit(200);
			}
		}
		if(cipher_code_required == 1){
			fseek(cipher_code_file, 0, SEEK_END);
			cipher_code_drive_size = ftell(cipher_code_file);

			if(cipher_code_size > cipher_code_drive_size - 2){
				printf("cipher-code size is %"PRIu64" bytes.  %s size is %"PRIu64" bytes.\n", cipher_code_size, cipher_code_file_name , cipher_code_drive_size - 2);
				fclose(cipher_code_file);
				fclose(infile);
				exit(200);
			}
			if(cipher_code_drive_size < cipher_code_size + cipher_code_address){
				printf("cipher_code boundary error. max start address is %"PRIu64".  %s size is %"PRIu64" bytes.\n", cipher_code_drive_size - cipher_code_size, cipher_code_file_name , cipher_code_drive_size);
				fclose(cipher_code_file);
				fclose(infile);
				exit(200);
			}
		}
		free(cipher_cb);
		cipher_cb = (uint8_t*)malloc(cipher_code_size * sizeof(uint8_t));
		fseek(cipher_code_file, cipher_code_address, SEEK_SET);
		fread(cipher_cb, cipher_code_size, 1, cipher_code_file);
		fclose(cipher_code_file);
	}
	if(cipher_code == 0){
		cipher_code_size = stat_code_size;

		uint8_t *randomizer;
		randomizer = (uint8_t*)malloc(cipher_code_size * sizeof(uint8_t));
		gettimeofday(&tiempo, NULL);
		srandom(time(NULL) * tiempo.tv_sec * tiempo.tv_usec * clock() + 2);
		for(uint64_t ctr = 0; ctr < cipher_code_size; ctr ++){
			randomizer[ctr] = random() % 0x100;
		}

		free(cipher_cb);
		cipher_cb = (uint8_t*)malloc(cipher_code_size * sizeof(uint8_t));
		gettimeofday(&tiempo, NULL);
		srandom(time(NULL) * tiempo.tv_sec * tiempo.tv_usec * clock() + randomizer[0]);
		for(uint64_t ctr = 0; ctr < cipher_code_size; ctr ++){
			cipher_cb[ctr] = random() % 0x100;
		}
		for(uint64_t ctr = 0; ctr < cipher_code_size; ctr ++){
			cipher_cb[ctr] = cipher_cb[ctr] ^ randomizer[ctr];
		}
		free(randomizer);

                /*adding cc extention*/
                uint8_t zero_finder = 0, byte_finder = 0;
                while(zero_finder < 64){
                        byte_finder = gen_lock_file_name[zero_finder];
                        if(byte_finder == 0){
                                cipher_output_name[zero_finder] = '.';
                                cipher_output_name[zero_finder + 1] = 'c';
                                cipher_output_name[zero_finder + 2] = 'c';
                                break;
                        }
                        if(byte_finder < 0x30 && byte_finder > 0){
                                byte_finder = 0x5f;
                        }
                        if(byte_finder < 0x41 && byte_finder > 0x39){
                                byte_finder = 0x5f;
                        }
                        if(byte_finder > 0x7a){
                                byte_finder = 0x5f;
                        }
                        if(byte_finder < 0x61 && byte_finder > 0x5a){
                                byte_finder = 0x5f;
                        }
                        cipher_output_name[zero_finder] = byte_finder;
                        zero_finder ++;
                }
                /*adding cc extention*/

		/*save cipher-code*/
		FILE* cipher_save;
		cipher_save = fopen(cipher_output_name,"wb");
		if(cipher_save == NULL){
			printf("couldn't save %s.\n", cipher_output_name);
			fclose(infile);
			free(cipher_cb);
			exit(255);
		}
		fseek(cipher_save, 0, SEEK_SET);
		fwrite(cipher_cb, cipher_code_size, 1, cipher_save);
		fclose(cipher_save);
		/*save cipher-code*/
	}
	/******************cipher code**********************/

	/*xor cipher code block*/
	for(uint64_t itr = 0; itr < cipher_code_size; itr ++){
		cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
		for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
			cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
		}
	}
	/*xor cipher code block*/

	/*header*/
	uint8_t *g_header;
	g_header = (uint8_t*)malloc(32 * sizeof(uint8_t));
	/*header*/

	/*rotors*/
	uint8_t **g_rotor;
	g_rotor = (uint8_t**)malloc(amount_of_rot_tab * sizeof(uint8_t*));
	for(uint64_t counter = 0; counter < amount_of_rot_tab; counter ++){
		g_rotor[counter] = (uint8_t*)malloc(256 * sizeof(uint8_t));
	}
	/*rotors*/

	/*tables*/
	uint8_t **g_table;
	g_table = (uint8_t**)malloc(amount_of_rot_tab * sizeof(uint8_t*));
	for(uint64_t counter = 0; counter < amount_of_rot_tab; counter ++){
		g_table[counter] = (uint8_t*)malloc(256 * sizeof(uint8_t));
	}
	/*tables*/

	/*rotor table swapper*/
	uint8_t **rt_swapper;
	rt_swapper = (uint8_t**)malloc(2 * sizeof(uint8_t*));
	for(uint64_t counter = 0; counter < 2; counter ++){
		rt_swapper[counter] = (uint8_t*)malloc(256 * sizeof(uint8_t));
	}
	/*rotor table swaper*/

	/*key*/
	uint8_t *g_key;
	g_key = (uint8_t*)malloc(keysize * sizeof(uint8_t));
	/*key*/

	/*transpotition*/
	uint8_t *g_transpotition;
	g_transpotition = (uint8_t*)malloc(transpotitionsize * sizeof(uint8_t));
	/*transpotition*/

	/*swap*/
	uint8_t *g_swap;
	g_swap = (uint8_t*)malloc(swapsize * sizeof(uint8_t));
	/*swap*/

	/*rotation*/
	uint8_t *g_rotation;
	g_rotation = (uint8_t*)malloc(rotationsize * sizeof(uint8_t));
	/*rotation*/

	//whole key
	uint8_t *whole_block;
	whole_block = (uint8_t*)malloc(whole_size * sizeof(uint8_t));
	//whole key

	//generation tracking
	tracking_counter = 0;
	uint8_t *tracking;
	tracking = (uint8_t*)malloc(256 * sizeof(uint8_t));
	//generation tracking

	//loads sizes
	for(uint8_t counter = 8; counter > 0; counter --){
		g_header[counter - 1] = transpotitionsize & 0x00000000000000ff;
		transpotitionsize = transpotitionsize >> 8;
	}

	for(uint8_t counter = 16; counter > 8; counter --){
		g_header[counter - 1] = keysize & 0x00000000000000ff;
		keysize = keysize >> 8;
	}

	for(uint8_t counter = 24; counter > 16; counter --){
		g_header[counter - 1] = swapsize & 0x00000000000000ff;
		swapsize = swapsize >> 8;
	}


	for(uint8_t counter = 32; counter > 24; counter --){
		g_header[counter - 1] = rotationsize & 0x00000000000000ff;
		rotationsize = rotationsize >> 8;
	}
	//loads sizes

	/*checksum array*/
	uint64_t *checksum_block;
	checksum_block = (uint64_t*)malloc(256 * sizeof(uint64_t));
	/*checksum array*/

///////////////////////////////////////////////////////ROTORS/////////////////////////////////////////////////////////////

	for(uint16_t counter = 0; counter < 256; counter ++){
		checksum_block[counter] = 0;
	}

	cipher_code_reset = 0;
	r_regenerated = 0;
	for(uint16_t rcounter = 0; rcounter < amount_of_rot_tab; rcounter ++){
		/*resets tracking*/
		for(uint16_t counter  = 0; counter < 256; counter ++){
			tracking[counter] = 5;
		}
		/*resets tracking*/

		/*reseed*/
		srandom(cipher_code_size
		* (cipher_cb[cipher_code_reset] + cipher_cb[cipher_code_reset + 1] + cipher_cb[cipher_code_reset + 2] + cipher_cb[cipher_code_reset + 3])
		* (cipher_cb[cipher_code_reset + 4] + cipher_cb[cipher_code_reset + 5] + cipher_cb[cipher_code_reset + 6]+ cipher_cb[cipher_code_reset + 7])
		* (cipher_cb[cipher_code_reset + 8] + cipher_cb[cipher_code_reset + 9] + cipher_cb[cipher_code_reset + 10]+ cipher_cb[cipher_code_reset + 11])
		* (cipher_cb[cipher_code_reset + 12] + cipher_cb[cipher_code_reset + 13] + cipher_cb[cipher_code_reset + 14]+ cipher_cb[cipher_code_reset + 15])
		* (cipher_cb[cipher_code_reset + 16] + cipher_cb[cipher_code_reset + 17] + cipher_cb[cipher_code_reset + 18]+ cipher_cb[cipher_code_reset + 19])
		- cipher_cb[cipher_code_reset + 20] + cipher_cb[cipher_code_reset + 21] - cipher_cb[cipher_code_reset + 22] + cipher_cb[cipher_code_reset + 23]
		* (cipher_cb[cipher_code_reset + 24] + cipher_cb[cipher_code_reset + 25] + cipher_cb[cipher_code_reset + 26]+ cipher_cb[cipher_code_reset + 27])
		* (cipher_cb[cipher_code_reset + 28] + cipher_cb[cipher_code_reset + 29] + cipher_cb[cipher_code_reset + 30]+ cipher_cb[cipher_code_reset + 31])
		);
		cipher_code_reset ++;
		if(cipher_code_reset >= cipher_code_size - 31){
			cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
			for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
				cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
			}
			cipher_code_reset = 0;

			cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
			for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
				cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
			}
		}
		/*reseed*/

		tracking_counter = 0;
		while(tracking_counter < 256){
			search_byte = random() % 0x100;
			if(tracking[search_byte] == 0){
				continue;
			}
			tracking[search_byte] = 0;
			g_rotor[rcounter][tracking_counter] = search_byte;

			tracking_counter ++;

			/*chechsum*/
			if(tracking_counter == 256){
				check_less = 64000;
				for(uint16_t counter = 0; counter < 256; counter = counter + 2){
					check_less = check_less + g_rotor[rcounter][counter];
					check_less = check_less - g_rotor[rcounter][counter + 1];
				}
				checksum_multi = 0;
				for(uint16_t counter = 0; counter < 255; counter ++){
					checksum_multi = checksum_multi + g_rotor[rcounter][counter] * g_rotor[rcounter][counter + 1];
				}
				checksum = 0;
				reverse_packet_counter = 255;
				for(uint16_t counter = 0; counter < 128; counter ++){
					checksum = checksum + g_rotor[rcounter][counter] * g_rotor[rcounter][reverse_packet_counter];
					reverse_packet_counter --;
				}
				checksum_block[rcounter] = checksum * checksum_multi * check_less;
			}

			if(tracking_counter == 256 && rcounter > 0){
				for(uint16_t counter = 0; counter < rcounter; counter ++){
					if(checksum_block[rcounter] == checksum_block[counter]){
						for(uint16_t scounter  = 0; scounter < 256; scounter ++){
							tracking[scounter] = 5;
						}
						tracking_counter = 0;
						r_regenerated ++;
						continue;
					}
				}
			}
			/*chechsum*/
		}
	}

	if(print_sum == 1){
		for(uint16_t counter  = 0; counter < 256; counter ++){
			printf("rotor: %"PRIu16".    SN: %"PRIu64".\n", counter, checksum_block[counter]);
		}
	}
	//generate rotors
///////////////////////////////////////////////////////ROTORS/////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////TABLES/////////////////////////////////////////////////////////////

	for(uint16_t counter = 0; counter < 256; counter ++){
		checksum_block[counter] = 0;
	}

	t_regenerated = 0;
	for(uint16_t rcounter = 0; rcounter < amount_of_rot_tab; rcounter ++){
		/*resets tracking*/
		for(uint16_t counter  = 0; counter < 256; counter ++){
			tracking[counter] = 5;
		}
		/*resets tracking*/

		/*reseed*/
		srandom(cipher_code_size
		* (cipher_cb[cipher_code_reset] + cipher_cb[cipher_code_reset + 1] + cipher_cb[cipher_code_reset + 2] + cipher_cb[cipher_code_reset + 3])
		* (cipher_cb[cipher_code_reset + 4] + cipher_cb[cipher_code_reset + 5] + cipher_cb[cipher_code_reset + 6]+ cipher_cb[cipher_code_reset + 7])
		* (cipher_cb[cipher_code_reset + 8] + cipher_cb[cipher_code_reset + 9] + cipher_cb[cipher_code_reset + 10]+ cipher_cb[cipher_code_reset + 11])
		* (cipher_cb[cipher_code_reset + 12] + cipher_cb[cipher_code_reset + 13] + cipher_cb[cipher_code_reset + 14]+ cipher_cb[cipher_code_reset + 15])
		* (cipher_cb[cipher_code_reset + 16] + cipher_cb[cipher_code_reset + 17] + cipher_cb[cipher_code_reset + 18]+ cipher_cb[cipher_code_reset + 19])
		- cipher_cb[cipher_code_reset + 20] + cipher_cb[cipher_code_reset + 21] - cipher_cb[cipher_code_reset + 22] + cipher_cb[cipher_code_reset + 23]
		* (cipher_cb[cipher_code_reset + 24] + cipher_cb[cipher_code_reset + 25] + cipher_cb[cipher_code_reset + 26]+ cipher_cb[cipher_code_reset + 27])
		* (cipher_cb[cipher_code_reset + 28] + cipher_cb[cipher_code_reset + 29] + cipher_cb[cipher_code_reset + 30]+ cipher_cb[cipher_code_reset + 31])
		);
		cipher_code_reset ++;
		if(cipher_code_reset >= cipher_code_size - 31){
			cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
			for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
				cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
			}
			cipher_code_reset = 0;

			cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
			for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
				cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
			}
		}
		/*reseed*/

		tracking_counter = 0;
		while(tracking_counter < 256){
			search_byte = random() % 0x100;
			if(tracking[search_byte] == 0){
				continue;
			}
			tracking[search_byte] = 0;
			g_table[rcounter][tracking_counter] = search_byte;

			tracking_counter ++;

			/*checksum*/
			if(tracking_counter == 256){
				check_less = 64000;
				for(uint16_t counter = 0; counter < 256; counter = counter + 2){
					check_less = check_less + g_table[rcounter][counter];
					check_less = check_less - g_table[rcounter][counter + 1];
				}
				checksum_multi = 0;
				for(uint16_t counter = 0; counter < 255; counter ++){
					checksum_multi = checksum_multi + g_table[rcounter][counter] * g_table[rcounter][counter + 1];
				}
				checksum = 0;
				reverse_packet_counter = 255;
				for(uint16_t counter = 0; counter < 128; counter ++){
					checksum = checksum + g_table[rcounter][counter] * g_table[rcounter][reverse_packet_counter];
					reverse_packet_counter --;
				}
				checksum_block[rcounter] = checksum * checksum_multi * check_less;
			}

			if(tracking_counter == 256 && rcounter > 0){
				for(uint16_t counter = 0; counter < rcounter; counter ++){
					if(checksum_block[rcounter] == checksum_block[counter]){
						for(uint16_t scounter  = 0; scounter < 256; scounter ++){
							tracking[scounter] = 5;
						}
						tracking_counter = 0;
						t_regenerated ++;
						continue;
					}
				}
			}
			/*checksum*/
		}
	}

	if(print_sum == 1){
		for(uint16_t counter = 0; counter < 256; counter ++){
			printf("table: %"PRIu16".    SN: %"PRIu64".\n", counter, checksum_block[counter]);
		}
	}
	//generate transpotition tables
///////////////////////////////////////////////////////TABLES/////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////SWAPPER/////////////////////////////////////////////////////////////
	/*swapp rotors and tables*/
	for(uint16_t swcounter = 0; swcounter < 2; swcounter ++){
		/*resets tracking*/
		for(uint16_t counter  = 0; counter < 256; counter ++){
			tracking[counter] = 5;
		}
		/*resets tracking*/

		/*reseed*/
		srandom(cipher_code_size
		* (cipher_cb[cipher_code_reset] + cipher_cb[cipher_code_reset + 1] + cipher_cb[cipher_code_reset + 2] + cipher_cb[cipher_code_reset + 3])
		* (cipher_cb[cipher_code_reset + 4] + cipher_cb[cipher_code_reset + 5] + cipher_cb[cipher_code_reset + 6]+ cipher_cb[cipher_code_reset + 7])
		* (cipher_cb[cipher_code_reset + 8] + cipher_cb[cipher_code_reset + 9] + cipher_cb[cipher_code_reset + 10]+ cipher_cb[cipher_code_reset + 11])
		* (cipher_cb[cipher_code_reset + 12] + cipher_cb[cipher_code_reset + 13] + cipher_cb[cipher_code_reset + 14]+ cipher_cb[cipher_code_reset + 15])
		* (cipher_cb[cipher_code_reset + 16] + cipher_cb[cipher_code_reset + 17] + cipher_cb[cipher_code_reset + 18]+ cipher_cb[cipher_code_reset + 19])
		- cipher_cb[cipher_code_reset + 20] + cipher_cb[cipher_code_reset + 21] - cipher_cb[cipher_code_reset + 22] + cipher_cb[cipher_code_reset + 23]
		* (cipher_cb[cipher_code_reset + 24] + cipher_cb[cipher_code_reset + 25] + cipher_cb[cipher_code_reset + 26]+ cipher_cb[cipher_code_reset + 27])
		* (cipher_cb[cipher_code_reset + 28] + cipher_cb[cipher_code_reset + 29] + cipher_cb[cipher_code_reset + 30]+ cipher_cb[cipher_code_reset + 31])
		);
		cipher_code_reset ++;
		if(cipher_code_reset >= cipher_code_size - 31){
			cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
			for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
				cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
			}
			cipher_code_reset = 0;

			cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
			for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
				cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
			}
		}
		/*reseed*/

		tracking_counter = 0;
		while(tracking_counter < 256){
			search_byte = random() % 0x100;
			if(tracking[search_byte] == 0){
				continue;
			}
			tracking[search_byte] = 0;
			rt_swapper[swcounter][tracking_counter] = search_byte;

			tracking_counter ++;

			/*checksum*/
			if(tracking_counter == 256){
				check_less = 64000;
				for(uint16_t counter = 0; counter < 256; counter = counter + 2){
					check_less = check_less + rt_swapper[swcounter][counter];
					check_less = check_less - rt_swapper[swcounter][counter + 1];
				}
				checksum_multi = 0;
				for(uint16_t counter = 0; counter < 255; counter ++){
					checksum_multi = checksum_multi + rt_swapper[swcounter][counter] * rt_swapper[swcounter][counter + 1];
				}
				checksum = 0;
				reverse_packet_counter = 255;
				for(uint16_t counter = 0; counter < 128; counter ++){
					checksum = checksum + rt_swapper[swcounter][counter] * rt_swapper[swcounter][reverse_packet_counter];
					reverse_packet_counter --;
				}
				checksum_block[swcounter] = checksum * checksum_multi * check_less;
			}

			if(tracking_counter == 256 && swcounter > 0){
				for(uint16_t counter = 0; counter < swcounter; counter ++){
					if(checksum_block[swcounter] == checksum_block[counter]){
						for(uint16_t scounter  = 0; scounter < 256; scounter ++){
							tracking[scounter] = 5;
						}
						tracking_counter = 0;
						continue;
					}
				}
			}
			/*checksum*/
		}
	}
	/*swapp rotors and tables*/
///////////////////////////////////////////////////////SWAPPER/////////////////////////////////////////////////////////////

	//generate key
	srandom(cipher_code_size
	* (cipher_cb[cipher_code_reset] + cipher_cb[cipher_code_reset + 1] + cipher_cb[cipher_code_reset + 2] + cipher_cb[cipher_code_reset + 3])
	* (cipher_cb[cipher_code_reset + 4] + cipher_cb[cipher_code_reset + 5] + cipher_cb[cipher_code_reset + 6]+ cipher_cb[cipher_code_reset + 7])
	* (cipher_cb[cipher_code_reset + 8] + cipher_cb[cipher_code_reset + 9] + cipher_cb[cipher_code_reset + 10]+ cipher_cb[cipher_code_reset + 11])
	* (cipher_cb[cipher_code_reset + 12] + cipher_cb[cipher_code_reset + 13] + cipher_cb[cipher_code_reset + 14]+ cipher_cb[cipher_code_reset + 15])
	* (cipher_cb[cipher_code_reset + 16] + cipher_cb[cipher_code_reset + 17] + cipher_cb[cipher_code_reset + 18]+ cipher_cb[cipher_code_reset + 19])
	- cipher_cb[cipher_code_reset + 20] + cipher_cb[cipher_code_reset + 21] - cipher_cb[cipher_code_reset + 22] + cipher_cb[cipher_code_reset + 23]
	* (cipher_cb[cipher_code_reset + 24] + cipher_cb[cipher_code_reset + 25] + cipher_cb[cipher_code_reset + 26]+ cipher_cb[cipher_code_reset + 27])
	* (cipher_cb[cipher_code_reset + 28] + cipher_cb[cipher_code_reset + 29] + cipher_cb[cipher_code_reset + 30]+ cipher_cb[cipher_code_reset + 31])
	);
	cipher_code_reset ++;
	if(cipher_code_reset >= cipher_code_size - 31){
		cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
		for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
			cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
		}
		cipher_code_reset = 0;

		cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
		for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
			cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
		}
	}
	for(uint64_t counter = 0; counter < keysize_o; counter ++){
		g_key[counter] = random() % 0x100;
	}
	//generate key

	//generate transpotition
	srandom(cipher_code_size
	* (cipher_cb[cipher_code_reset] + cipher_cb[cipher_code_reset + 1] + cipher_cb[cipher_code_reset + 2] + cipher_cb[cipher_code_reset + 3])
	* (cipher_cb[cipher_code_reset + 4] + cipher_cb[cipher_code_reset + 5] + cipher_cb[cipher_code_reset + 6]+ cipher_cb[cipher_code_reset + 7])
	* (cipher_cb[cipher_code_reset + 8] + cipher_cb[cipher_code_reset + 9] + cipher_cb[cipher_code_reset + 10]+ cipher_cb[cipher_code_reset + 11])
	* (cipher_cb[cipher_code_reset + 12] + cipher_cb[cipher_code_reset + 13] + cipher_cb[cipher_code_reset + 14]+ cipher_cb[cipher_code_reset + 15])
	* (cipher_cb[cipher_code_reset + 16] + cipher_cb[cipher_code_reset + 17] + cipher_cb[cipher_code_reset + 18]+ cipher_cb[cipher_code_reset + 19])
	- cipher_cb[cipher_code_reset + 20] + cipher_cb[cipher_code_reset + 21] - cipher_cb[cipher_code_reset + 22] + cipher_cb[cipher_code_reset + 23]
	* (cipher_cb[cipher_code_reset + 24] + cipher_cb[cipher_code_reset + 25] + cipher_cb[cipher_code_reset + 26]+ cipher_cb[cipher_code_reset + 27])
	* (cipher_cb[cipher_code_reset + 28] + cipher_cb[cipher_code_reset + 29] + cipher_cb[cipher_code_reset + 30]+ cipher_cb[cipher_code_reset + 31])
	);
	cipher_code_reset ++;
	if(cipher_code_reset >= cipher_code_size - 31){
		cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
		for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
			cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
		}
		cipher_code_reset = 0;

		cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
		for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
			cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
		}
	}
	for(uint64_t counter = 0; counter < transpotitionsize_o; counter ++){
		g_transpotition[counter] = random() % 0x100;
	}
	//generate transpotition

	//generate swap
	srandom(cipher_code_size
	* (cipher_cb[cipher_code_reset] + cipher_cb[cipher_code_reset + 1] + cipher_cb[cipher_code_reset + 2] + cipher_cb[cipher_code_reset + 3])
	* (cipher_cb[cipher_code_reset + 4] + cipher_cb[cipher_code_reset + 5] + cipher_cb[cipher_code_reset + 6]+ cipher_cb[cipher_code_reset + 7])
	* (cipher_cb[cipher_code_reset + 8] + cipher_cb[cipher_code_reset + 9] + cipher_cb[cipher_code_reset + 10]+ cipher_cb[cipher_code_reset + 11])
	* (cipher_cb[cipher_code_reset + 12] + cipher_cb[cipher_code_reset + 13] + cipher_cb[cipher_code_reset + 14]+ cipher_cb[cipher_code_reset + 15])
	* (cipher_cb[cipher_code_reset + 16] + cipher_cb[cipher_code_reset + 17] + cipher_cb[cipher_code_reset + 18]+ cipher_cb[cipher_code_reset + 19])
	- cipher_cb[cipher_code_reset + 20] + cipher_cb[cipher_code_reset + 21] - cipher_cb[cipher_code_reset + 22] + cipher_cb[cipher_code_reset + 23]
	* (cipher_cb[cipher_code_reset + 24] + cipher_cb[cipher_code_reset + 25] + cipher_cb[cipher_code_reset + 26]+ cipher_cb[cipher_code_reset + 27])
	* (cipher_cb[cipher_code_reset + 28] + cipher_cb[cipher_code_reset + 29] + cipher_cb[cipher_code_reset + 30]+ cipher_cb[cipher_code_reset + 31])
	);
	cipher_code_reset ++;
	if(cipher_code_reset >= cipher_code_size - 31){
		cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
		for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
			cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
		}
		cipher_code_reset = 0;

		cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
		for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
			cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
		}
	}
	for(uint64_t counter = 0; counter < swapsize_o; counter ++){
		g_swap[counter] = random() % 0x100;
	}
	//generate swap

	//generate rotation
	srandom(cipher_code_size
	* (cipher_cb[cipher_code_reset] + cipher_cb[cipher_code_reset + 1] + cipher_cb[cipher_code_reset + 2] + cipher_cb[cipher_code_reset + 3])
	* (cipher_cb[cipher_code_reset + 4] + cipher_cb[cipher_code_reset + 5] + cipher_cb[cipher_code_reset + 6]+ cipher_cb[cipher_code_reset + 7])
	* (cipher_cb[cipher_code_reset + 8] + cipher_cb[cipher_code_reset + 9] + cipher_cb[cipher_code_reset + 10]+ cipher_cb[cipher_code_reset + 11])
	* (cipher_cb[cipher_code_reset + 12] + cipher_cb[cipher_code_reset + 13] + cipher_cb[cipher_code_reset + 14]+ cipher_cb[cipher_code_reset + 15])
	* (cipher_cb[cipher_code_reset + 16] + cipher_cb[cipher_code_reset + 17] + cipher_cb[cipher_code_reset + 18]+ cipher_cb[cipher_code_reset + 19])
	- cipher_cb[cipher_code_reset + 20] + cipher_cb[cipher_code_reset + 21] - cipher_cb[cipher_code_reset + 22] + cipher_cb[cipher_code_reset + 23]
	* (cipher_cb[cipher_code_reset + 24] + cipher_cb[cipher_code_reset + 25] + cipher_cb[cipher_code_reset + 26]+ cipher_cb[cipher_code_reset + 27])
	* (cipher_cb[cipher_code_reset + 28] + cipher_cb[cipher_code_reset + 29] + cipher_cb[cipher_code_reset + 30]+ cipher_cb[cipher_code_reset + 31])
	);
	cipher_code_reset ++;
	if(cipher_code_reset >= cipher_code_size - 31){
		cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
		for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
			cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
		}
		cipher_code_reset = 0;

		cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
		for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
			cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
		}
	}
	for(uint64_t counter = 0; counter < rotationsize_o; counter ++){
		g_rotation[counter] = random() % 0x100;
	}
	//generate rotation

	/*load into final array*/
		/*loads header*/
		whole_counter = 0;
		for(uint64_t counter = 0; counter < 32; counter ++){
			whole_block[whole_counter] = g_header[counter];
			whole_counter ++;
		}
		/*loads header*/

		/*loads rotors*/
		for(uint64_t rcounter = 0; rcounter < amount_of_rot_tab; rcounter ++){
			for(uint64_t counter = 0; counter < 256; counter ++){
				whole_block[whole_counter] = g_rotor[rt_swapper[0][rcounter]][counter];
				whole_counter ++;
			}
		}
		/*loads rotors*/

		/*loads tables*/
		for(uint64_t tcounter = 0; tcounter < amount_of_rot_tab; tcounter ++){
			for(uint64_t counter = 0; counter < 256; counter ++){
				whole_block[whole_counter] = g_table[rt_swapper[1][tcounter]][counter];
				whole_counter ++;
			}
		}
		/*loads tables*/


		/*loads key*/
		for(uint64_t counter = 0; counter < keysize_o; counter ++){
			whole_block[whole_counter] = g_key[counter] ^ cipher_cb[cipher_code_reset];
			whole_counter ++;
			cipher_code_reset ++;
			if(cipher_code_reset >= cipher_code_size){
				cipher_code_reset = 0;

				cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
				for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
					cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
				}
			}
		}
		/*loads key*/

		/*loads transpotition*/
		for(uint64_t counter = 0; counter < transpotitionsize_o; counter ++){
			whole_block[whole_counter] = g_transpotition[counter] ^ cipher_cb[cipher_code_reset];
			whole_counter ++;
			cipher_code_reset ++;
			if(cipher_code_reset >= cipher_code_size){
				cipher_code_reset = 0;

				cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
				for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
					cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
				}
			}
		}
		/*loads transpotition*/

		/*loads swap*/
		for(uint64_t counter = 0; counter < swapsize_o; counter ++){
			whole_block[whole_counter] = g_swap[counter] ^ cipher_cb[cipher_code_reset];
			whole_counter ++;
			cipher_code_reset ++;
			if(cipher_code_reset >= cipher_code_size){
				cipher_code_reset = 0;

				cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
				for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
					cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
				}
			}
		}
		/*loads swap*/

		/*loads rotation*/
		for(uint64_t counter = 0; counter < rotationsize_o; counter ++){
			whole_block[whole_counter] = g_rotation[counter] ^ cipher_cb[cipher_code_reset];
			whole_counter ++;
			cipher_code_reset ++;
			if(cipher_code_reset >= cipher_code_size){
				cipher_code_reset = 0;

				cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
				for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
					cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
				}
			}
		}
		/*loads rotation*/
	/*load into final array*/

	/*xor generated key block*/
	key_counter_reset = whole_size - 1;
	for(uint64_t counter = 0; counter < (whole_size / 2); counter ++){
		whole_block[counter] = whole_block[counter] ^ whole_block[key_counter_reset];
		key_counter_reset --;
	}
	key_counter_reset = whole_size / 2;
	for(uint64_t counter = 0; counter < (whole_size / 2); counter ++){
		whole_block[key_counter_reset] = whole_block[key_counter_reset] ^ whole_block[counter];
		key_counter_reset ++;
	}
	key_counter_reset = whole_size - 1;
	for(uint64_t counter = 0; counter < (whole_size / 2); counter ++){
		whole_block[counter] = whole_block[counter] ^ whole_block[key_counter_reset];
		key_counter_reset --;
	}
	key_counter_reset = whole_size / 2;
	for(uint64_t counter = 0; counter < (whole_size / 2); counter ++){
		whole_block[key_counter_reset] = whole_block[key_counter_reset] ^ whole_block[counter];
		key_counter_reset ++;
	}
	/*xor generated key block*/

	//write
	fseek(infile, lock_address, SEEK_SET);
	fwrite(whole_block, whole_size, 1, infile);
	//write

	free(cipher_cb);

	free(g_header);
	free(g_rotor);
	free(g_table);
	free(rt_swapper);
	free(g_key);
	free(g_transpotition);
	free(g_swap);
	free(g_rotation);

	free(checksum_block);
	free(whole_block);
	free(tracking);
	fclose(infile);

	printf("regenerated rotors: %"PRIu64".   regenerated tables: %"PRIu64".\n", r_regenerated, t_regenerated);
	printf("rotors-blk: %"PRIu64".  tables-blk: %"PRIu64".  key-blk: %"PRIu64".  transpo-blk: %"PRIu64".  swap-blk: %"PRIu64".  rotation-blk: %"PRIu64".\n",rotor_b_size, transpotition_b_size, keysize_o, transpotitionsize_o, swapsize_o, rotationsize_o);
	printf("cipher-lock-256bit size: %"PRIu64".  addresses: %"PRIu64" - %"PRIu64".\n", whole_counter, lock_address, lock_address + (whole_size - 1));
	if(cipher_code == 0){
		printf("random generated cc-block size %"PRIu64" bytes.\n", cipher_code_size);
	}
	exit(0);
        ////////////////////////////////////*generates lock*////////////////////////////////////////
}
