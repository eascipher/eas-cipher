void consoleInput(uint8_t *cells8, uint16_t *cells16, uint64_t *cells64, char **strg, char **cargv){
	///////////////////////////* help */
	if(cargc == 1){
		 printf(
	"eas-256bit-cipher.\n"
	"------------------\n"
	"     -g       generate lock file.\n"
	"     -c       cipher.\n"
	"     -d       decipher.\n"
	"     -obs     output block shift. {1 - 50,000,000}.\n"
	"     -l       lock file.\n"
	"     -cc      cipher code. {300 - 500,000}.\n"
	"     -i       input file.\n"
	"     -o       output file.\n"
	"     -z       print drive size.\n"
	"     -cl      calculate lock size. {1 - 500,000}.\n"
	"     -vs      verify and print tables & rotors SN.\n"
	"     -v       verify lock.\n"
	"     -h       print hex.\n"
	"     -H       print upper hex.\n"
	"     -cn      column number.\n\n"

	"cipher block mode.\n"
	"------------------\n"
	"     -ssr     single-swap-rotation/block     ~ 6sec/100M.\n"
	"     -msr     multi-swap-rotation/block      ~12sec/100M.\n"
	"     -iter    iterations/block ~ 6sec/100M x {1 - 50,000,000}.\n\n"

	"key regeneration.\n"
	"-----------------\n"
	"     -ss      shift substitution key. {1 - 50,000,000}.\n"
	"     -sx      xor location when shiftting substitution key. {1 - 50,000,000}.\n"
	"     -ts      shift transposition key. {1 - 50,000,000}.\n"
	"     -tx      xor location when shiftting transposition key. {1 - 50,000,000}.\n"
	"     -fs      shift first key. {1 - 50,000,000}.\n"
	"     -fx      xor location when shiftting first key. {1 - 50,000,000}.\n\n"

	"eas-256bit-cipher raw_drive.\n"
	"----------------------------\n"
	"     -la      lock address.\n"
	"     -ls      lock size. {1 - 500,000}.\n"
	"     -ia      input address.\n"
	"     -is      input size. {-c 1 - 2,000,000,000.}  {-d 256 - 2,500,000,000}.\n"
	"     -oa      output address.\n"
	"     -cca     cipher code address.\n"
	"     -ccs     cipher code size. {300 - 500,000}.\n"
	);
		exit(0);
	}
	/* help */


	/*+++++++++++++++++++++++++print serial number*/
	strcpy(term_args, "-vs");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			arg_number = counter + 1;
			print_serial = 1;
			verify_rt = 1;
			cipher_option = 0;
			decipher_option = 0;
			if(arg_number == cargc){
				printf("-vs: ?\n");
				exit(1);
			}
			strcpy(lock_file_name, cargv[counter + 1]);
		}
	}
	if(not_found == 1){
		print_serial = 0;
	}
	/*+++++++++++++++++++++++++print serial number*/

	/*+++++++++++++++++++++++++print serial number*/
	if(print_serial == 0){
		strcpy(term_args, "-v");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				arg_number = counter + 1;
				verify_rt = 1;
				cipher_option = 0;
				decipher_option = 0;
				if(arg_number == cargc){
					printf("-v: ?\n");
					exit(1);
				}
				strcpy(lock_file_name, cargv[counter + 1]);
			}
		}
		if(not_found == 1){
			verify_rt = 0;
		}
	}
	/*+++++++++++++++++++++++++print serial number*/

	/*+++++++++++++++++++++++++calculates key size*/
	strcpy(term_args, "-cl");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			arg_number = counter + 1;
			if(arg_number == cargc){
				printf("-cl: ?\n");
				exit(1);
			}
			key_b_size = atoll(cargv[counter + 1]);
			if(key_b_size < 1 || key_b_size > 500000){
				printf("-cl: 1 - 500,000\n");
				exit(1);
			}
			key_b_size = (key_b_size * (32 * (2 + (2 * 8)))) + (65536 * 2) + 32;
			printf("cipher-lock-256bit is %"PRIu64" bytes.\n", key_b_size);
			exit(0);
		}
	}
	/*+++++++++++++++++++++++++calculates key size*/

	/*+++++++++++++++++++++++++print file size++++++++++++++++++++++*/
	strcpy(term_args, "-z");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			arg_number = counter + 1;
			if(arg_number == cargc){
				printf("please select a drive.\n");
				exit(1);
			}
			strcpy(input_file_name, cargv[counter + 1]);
			FILE *drive;
			drive = fopen(input_file_name, "r+b");
			if(drive == NULL){
				printf("couldn't open %s.\n", input_file_name);
				exit(1);
			}
			fseek(drive, 0, SEEK_END);
			drive_size = ftell(drive);
			printf("drive %s is %"PRIu64" bytes.\n", input_file_name, drive_size);
			fclose(drive);
			exit(0);
		}
	}
	/*+++++++++++++++++++++++++print file size++++++++++++++++++++++*/

	/*lock generate*/
	strcpy(term_args, "-g");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			gen_lock_option = 1;
		}
	}
	if(not_found == 1){
		gen_lock_option = 0;
	}

	/////////////////////////////////////////GEN LOCK////////////////////////////////*-------if found--*/
	if(gen_lock_option == 1){
	        /*help*/
	        if(cargc == 2){
			 printf("cipher-lock-256bit generator.\n"
				"-----------------------------\n"
	               	 	"     -o       lock name.\n"
				"     -cc      cipher code.\n"
				"     -stc     static cipher-code size. {300 - 500,000}.\n"
	                	"     -gls     generated lock size. {1 - 500,000}.\n"
	                	"     -sn      print tables & rotors SN.\n\n"

				"cipher-lock-256bit generator raw_drive.\n"
				"---------------------------------------\n"
				"     -la      lock address.\n"
				"     -cca     cipher code address.\n"
				"     -ccs     cipher code size. {300 - 500,000}.\n");
	                exit(0);
	        }
	        /*help*/

	        /*key lenght*/
	        strcpy(term_args, "-gls");
	        not_found = 1;
	        for(uint8_t counter = 0; counter < cargc; counter ++){
	                comparacion = strcmp(term_args, cargv[counter]);
	                if(comparacion == 0){
	                        not_found = 0;
	                        arg_number = counter + 1;
	                        if(arg_number == cargc){
	                                printf("-gls: ?\n");
	                                exit(1);
	                        }
	                        kbsize = atoll(cargv[counter + 1]);
	                        if(kbsize < 1 || kbsize > 500000){
	                                printf("-gls: 1 - 500,000\n");
	                                exit(1);
	                        }
	                }
	        }
	        if(not_found == 1){
	                kbsize = 1;
	        }
	        /*key lenght*/

	        /*key lenght*/
	        strcpy(term_args, "-stc");
	        not_found = 1;
	        for(uint8_t counter = 0; counter < cargc; counter ++){
	                comparacion = strcmp(term_args, cargv[counter]);
	                if(comparacion == 0){
	                        not_found = 0;
	                        arg_number = counter + 1;
	                        if(arg_number == cargc){
	                                printf("-stc: ?\n");
	                                exit(1);
	                        }
	                        stat_code_size = atoll(cargv[counter + 1]);
	                        if(stat_code_size < 300 || stat_code_size > 500000){
	                                printf("-stc: 300 - 500,000\n");
	                                exit(1);
	                        }
	                }
	        }
	        if(not_found == 1){
	                stat_code_size = 300;
	        }
	        /*key lenght*/

	        /*+++++++++++++++++++++++++search for lock name*/
	        strcpy(term_args, "-o");
	        not_found = 1;
	        for(uint8_t counter = 0; counter < cargc; counter ++){
	                comparacion = strcmp(term_args, cargv[counter]);
	                if(comparacion == 0){
	                        not_found = 0;
	                        arg_number = counter + 1;
	                        if(arg_number == cargc){
	                                printf("-o: ?\n");
	                                exit(1);
	                        }
	                        strcpy(gen_lock_file_name, cargv[counter + 1]);
	                }
	        }
	        if(not_found == 1){
	                printf("please select an output cipher-lock-256bit. -o.\n");
	                exit(1);
	        }
	        /*+++++++++++++++++++++++++search for lock name*/

		/************************cipher code**********************/
		strcpy(term_args, "-cc");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				cipher_code = 1;
				arg_number = counter + 1;
				if(arg_number == cargc){
					printf("-cc: ?\n");
					exit(1);
				}
				strcpy(cipher_code_file_name, cargv[counter + 1]);
			}
		}
		if(not_found == 1){
			cipher_code = 0;
		}
		/************************cipher code**********************/

		/************************test output************************/
		if(gen_lock_file_name[0] == 0x2d){
			printf("%s is an invalid input.\n", gen_lock_file_name);
			exit(101);
		}
		lock_required = 0;
		for(uint8_t counter  = 0; counter < 6; counter ++){
			test[counter] = gen_lock_file_name[counter];
		}
		test[5] = 0x00;
		strcpy(cmp, "/dev/");
		cmp[5] = 0x00;
		comparacion = strcmp(cmp, test);
		if(comparacion == 0){
			lock_required = 1;
		}
		/************************test output****************************/
		/************************test cipher code************************/
		if(cipher_code == 1){
			if(cipher_code_file_name[0] == 0x2d){
				printf("%s is an invalid input.\n", cipher_code_file_name);
				exit(102);
			}
			cipher_code_required = 0;
			for(uint8_t counter  = 0; counter < 6; counter ++){
				test[counter] = cipher_code_file_name[counter];
			}
			test[5] = 0x00;
			strcpy(cmp, "/dev/");
			cmp[5] = 0x00;
			comparacion = strcmp(cmp, test);
			if(comparacion == 0){
				cipher_code_required = 1;
			}
		}
		/************************test cipher code****************************/

		/***************************cipher code addresses and size****************/
		cipher_code_address = 0;
		if(cipher_code_required == 1){
			//address
			strcpy(term_args, "-cca");
			not_found = 1;
			for(uint8_t counter = 0; counter < cargc; counter ++){
				comparacion = strcmp(term_args, cargv[counter]);
				if(comparacion == 0){
					not_found = 0;
					arg_number = counter + 1;
					if(arg_number == cargc){
						printf("-cca: ?\n");
						exit(1);
					}
					cipher_code_address = atoll(cargv[counter + 1]);
					if(cipher_code_address < 1 || cipher_code_address > 1000000000000000000){
						printf("-cca: 1 - 1,000,000,000,000,000,000\n");
						exit(1);
					}
				}
			}
			if(not_found == 1){
				printf("please select cipher-code address. -cca.\n");
				exit(1);
			}
			//size
			strcpy(term_args, "-ccs");
			not_found = 1;
			for(uint8_t counter = 0; counter < cargc; counter ++){
				comparacion = strcmp(term_args, cargv[counter]);
				if(comparacion == 0){
					not_found = 0;
					arg_number = counter + 1;
					if(arg_number == cargc){
						printf("-ccs: ?\n");
						exit(1);
					}
					cipher_code_size = atoll(cargv[counter + 1]);
					if(cipher_code_size < 300 || cipher_code_size > 500000){
						printf("-ccs: 300 - 500,000.\n");
						exit(1);
					}
				}
			}
			if(not_found == 1){
				printf("please select cipher-code size. -ccs.\n");
				exit(222);
			}
		}
		/***************************cipher code addresses and size****************/

		/*+++++++++++++++++++ output required+++++++++++++++++++++++*/
		lock_address = 0;
		strcpy(output_mode, "wb");
		if(lock_required == 1){
			strcpy(output_mode, "r+b");
			//address
			strcpy(term_args, "-la");
			not_found = 1;
			for(uint8_t counter = 0; counter < cargc; counter ++){
				comparacion = strcmp(term_args, cargv[counter]);
				if(comparacion == 0){
					not_found = 0;
					arg_number = counter + 1;
					if(arg_number == cargc){
						printf("-la: ?\n");
						exit(1);
					}
					lock_address = atoll(cargv[counter + 1]);
					if(lock_address < 1 || lock_address > 1000000000000000000){
						printf("-la: 1 - 1,000,000,000,000,000,000\n");
						exit(1);
					}
				}
			}
			if(not_found == 1){
				printf("please select cipher-lock-256bit address. -la.\n");
				exit(1);
			}
		}
		/*+++++++++++++++++++ output required+++++++++++++++++++++++*/

	        /*+++++++++++++++++++++++++print checksum*/
	        strcpy(term_args, "-sn");
	        not_found = 1;
	        for(uint8_t counter = 0; counter < cargc; counter ++){
	                comparacion = strcmp(term_args, cargv[counter]);
	                if(comparacion == 0){
	                        not_found = 0;
				print_sum = 1;
	                }
	        }
	        if(not_found == 1){
			print_sum = 0;
	        }
	        /*+++++++++++++++++++++++++print checksum*/

	        ////////////////////////////////////*generates lock*////////////////////////////////////////~~0
		call_gen_lock = 1;
		return;
	        ////////////////////////////////////*generates lock*////////////////////////////////////////

	}
	/////////////////////////////////////////GEN LOCK////////////////////////////////*-------if found--*/

	/*+++++++++++++++++++++++++search for covertion type*/
	if(verify_rt == 0){
		strcpy(term_args, "-c");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				cipher_option = 1;
				decipher_option = 0;
				not_found = 0;
			}
		}
		if(not_found == 1){
			strcpy(term_args, "-d");
			for(uint8_t counter = 0; counter < cargc; counter ++){
				comparacion = strcmp(term_args, cargv[counter]);
				if(comparacion == 0){
					decipher_option = 1;
					cipher_option = 0;
					not_found = 0;
				}
			}
		}
		if(not_found == 1){
			printf("please select an operation: -c -d.\n");
			exit(1);
		}

		single_rotation = 0;
		multi_rotation = 0;
		strcpy(term_args, "-ssr");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				single_rotation = 1;
				not_found = 0;
			}
		}
		if(not_found == 1){
			strcpy(term_args, "-msr");
			for(uint8_t counter = 0; counter < cargc; counter ++){
				comparacion = strcmp(term_args, cargv[counter]);
				if(comparacion == 0){
					multi_rotation = 1;
					not_found = 0;
				}
			}
		}
		if(not_found == 1){
			strcpy(term_args, "-iter");
			for(uint8_t counter = 0; counter < cargc; counter ++){
				comparacion = strcmp(term_args, cargv[counter]);
				if(comparacion == 0){
					multimixed_rotation = 1;
					not_found = 0;
			                arg_number = counter + 1;
			                if(arg_number == cargc){
			                        printf("-iter: ?\n");
			                        exit(1);
			                }
			                multimixed_turns = atoll(cargv[counter + 1]);
			                if(multimixed_turns < 1 || multimixed_turns > 50000000){
			                        printf("-iter: 1 - 50,000,000\n");
			                        exit(1);
			                }

				}
			}
		}
		if(not_found == 1){
			printf("please select a block cipher mode: -ssr -msr -iter.\n");
			exit(1);
		}

	}
	/*+++++++++++++++++++++++++search for covertion type*/

	/*+++++++++++++++++++++++++search for key file*/
	if(verify_rt == 0){
		strcpy(term_args, "-l");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				arg_number = counter + 1;
				if(arg_number == cargc){
					printf("-l: ?\n");
					exit(1);
				}
				strcpy(lock_file_name, cargv[counter + 1]);
			}
		}
		if(not_found == 1){
			printf("please select a cipher-lock-256bit. -l.\n");
			exit(1);
		}

		strcpy(term_args, "-cc");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				cipher_code = 1;
				arg_number = counter + 1;
				if(arg_number == cargc){
					printf("-cc: ?\n");
					exit(1);
				}
				strcpy(cipher_code_file_name, cargv[counter + 1]);
			}
		}
		if(not_found == 1){
			cipher_code = 0;
		}
	}
	/*+++++++++++++++++++++++++search for key file*/

	/*+++++++++++++++++++++++++search for input*/
	strcpy(term_args, "-i");
	not_found = 1;
	console_input = 0;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			arg_number = counter + 1;
			if(arg_number == cargc){
				printf("-i: ?\n");
				exit(1);
			}
			strcpy(input_file_name, cargv[counter + 1]);
		}
	}

	if(not_found == 1){
		console_input = 1;
	}
	if(console_input == 1 && decipher_option == 1){
		printf("please select an input. -i.\n");
		exit(33);
	}
	/*+++++++++++++++++++++++++search for input*/

	/*+++++++++++++++++++++++++search for output type*/
	strcpy(term_args, "-o");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			print_option = 0;
			arg_number = counter + 1;
			if(arg_number == cargc){
				printf("-o: ?\n");
				exit(1);
			}
			strcpy(output_file_name, cargv[counter + 1]);
		}
	}

	if(not_found == 1){
		print_option = 1;
	}
	/*+++++++++++++++++++++++++search for output type*/

	/*++++++++++++++++print hex decrip++++++++++++++++++*/
	if(print_option == 1){
		strcpy(term_args, "-h");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				print_hex = 1;
			}
		}
		if(not_found == 1){
			print_hex = 0;
		}

		if(print_hex == 0){
			strcpy(term_args, "-H");
			not_found = 1;
			for(uint8_t counter = 0; counter < cargc; counter ++){
				comparacion = strcmp(term_args, cargv[counter]);
				if(comparacion == 0){
					not_found = 0;
					print_upp_hex = 1;
				}
			}
			if(not_found == 1){
				print_upp_hex = 0;
			}
		}

		strcpy(term_args, "-cn");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				arg_number = counter + 1;
				if(arg_number == cargc){
					printf("-cn: ?\n");
					exit(1);
				}
				column_number = atoll(cargv[counter + 1]);
				if(column_number < 4 || column_number > 256){
					printf("-cn: 4 - 256\n");
					exit(1);
				}
				if((column_number % 4) != 0){
					printf("-cn must be multiples of 4.\n");
					exit(127);
				}
			}
		}
		if(not_found == 1){
			column_number = 80;
		}
	}
	/*++++++++++++++++print hex decrip++++++++++++++++++*/

	/*+++++++++++++++++++++++++search for substitution shift times*/
	strcpy(term_args, "-ss");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			arg_number = counter + 1;
			if(arg_number == cargc){
				printf("-ss: ?\n");
				exit(1);
			}
			sub_shift_times = atoll(cargv[counter + 1]);
			if(sub_shift_times < 2 || sub_shift_times > 50000000){
				printf("-ss: 2 - 50,000,000\n");
				exit(1);
			}
		}
	}
	if(not_found == 1){
		sub_shift_times = 1;
	}
	/*+++++++++++++++++++++++++search for substitution shift times*/

	/*+++++++++++++++++++++++++search for substitution key shift location*/
	strcpy(term_args, "-sx");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			arg_number = counter + 1;
			if(arg_number == cargc){
				printf("-sx: ?\n");
				exit(1);
			}
			subkey_shift_location = atoll(cargv[counter + 1]);
			if(subkey_shift_location < 1 || subkey_shift_location > 50000000){
				printf("-sx: 1 - 50,000,000\n");
				exit(1);
			}
		}
	}
	if(not_found == 1){
		subkey_shift_location = 0;
	}
	/*+++++++++++++++++++++++++search for substitution key shift location*/
	/*+++++++++++++++++++++++++search for transposition shift times*/
	strcpy(term_args, "-ts");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			arg_number = counter + 1;
			if(arg_number == cargc){
				printf("-ts: ?\n");
				exit(1);
			}
			tra_shift_times = atoll(cargv[counter + 1]);
			if(tra_shift_times < 2 || tra_shift_times > 50000000){
				printf("-ts: 2 - 50,000,000\n");
				exit(1);
			}
		}
	}
	if(not_found == 1){
		tra_shift_times = 1;
	}
	/*+++++++++++++++++++++++++search for transposition shift times*/

	/*+++++++++++++++++++++++++search for transposition shift location*/
	strcpy(term_args, "-tx");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			arg_number = counter + 1;
			if(arg_number == cargc){
				printf("-tx: ?\n");
				exit(1);
			}
			trakey_shift_location = atoll(cargv[counter + 1]);
			if(trakey_shift_location < 1 || trakey_shift_location > 50000000){
				printf("-tx: 1 - 50,000,000\n");
				exit(1);
			}
		}
	}
	if(not_found == 1){
		trakey_shift_location = 0;
	}
	/*+++++++++++++++++++++++++search for transposition shift location*/
	/*+++++++++++++++++++++++++search for first shift times*/
	strcpy(term_args, "-fs");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			arg_number = counter + 1;
			if(arg_number == cargc){
				printf("-fs: ?\n");
				exit(1);
			}
			first_shift_times = atoll(cargv[counter + 1]);
			if(first_shift_times < 2 || first_shift_times > 50000000){
				printf("-fs: 2 - 50,000,000\n");
				exit(1);
			}
		}
	}
	if(not_found == 1){
		first_shift_times = 1;
	}
	/*+++++++++++++++++++++++++search for first shift times*/

	/*+++++++++++++++++++++++++search for first key shift location*/
	strcpy(term_args, "-fx");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			arg_number = counter + 1;
			if(arg_number == cargc){
				printf("-fx: ?\n");
				exit(1);
			}
			firstkey_shift_location = atoll(cargv[counter + 1]);
			if(firstkey_shift_location < 1 || firstkey_shift_location > 50000000){
				printf("-fx: 1 - 50,000,000\n");
				exit(1);
			}
		}
	}
	if(not_found == 1){
		firstkey_shift_location = 0;
	}
	/*+++++++++++++++++++++++++search for first key shift location*/
	/*+++++++++++++++++++++++++output block shift+++++++++++++++++*/
	strcpy(term_args, "-obs");
	not_found = 1;
	for(uint8_t counter = 0; counter < cargc; counter ++){
		comparacion = strcmp(term_args, cargv[counter]);
		if(comparacion == 0){
			not_found = 0;
			arg_number = counter + 1;
			if(arg_number == cargc){
				printf("-obs: ?\n");
				exit(1);
			}
			oblock_shift = atoll(cargv[counter + 1]);
			if(oblock_shift < 1 || oblock_shift > 50000000){
				printf("-obs: 1 - 50,000,000\n");
				exit(1);
			}
		}
	}
	if(not_found == 1){
		oblock_shift = 0;
	}
	/*+++++++++++++++++++++++++output block shift+++++++++++++++++*/

	/*==================================================================*/
	/***************************test input file**************************/
	if(input_file_name[0] == 0x2d){
		printf("%s is an invalid input.\n", input_file_name);
		exit(101);
	}
	input_required = 0;
	for(uint8_t counter  = 0; counter < 6; counter ++){
		test[counter] = input_file_name[counter];
	}
	test[5] = 0x00;
	strcpy(cmp, "/dev/");
	cmp[5] = 0x00;
	comparacion = strcmp(cmp, test);
	if(comparacion == 0){
		input_required = 1;
	}
	/***************************test input file**************************/

	/***************************test output file**************************/
	if(output_file_name[0] == 0x2d){
		printf("%s is an invalid output.\n", output_file_name);
		exit(101);
	}
	output_required = 0;
	for(uint8_t counter  = 0; counter < 6; counter ++){
		test[counter] = output_file_name[counter];
	}
	test[5] = 0x00;
	strcpy(cmp, "/dev/");
	cmp[5] = 0x00;
	comparacion = strcmp(cmp, test);
	if(comparacion == 0){
		output_required = 1;
	}
	/***************************test output file**************************/

	/***************************test lock file**************************/
	if(lock_file_name[0] == 0x2d){
		printf("%s is an invalid cipher-lock-256bit.\n", lock_file_name);
		exit(101);
	}
	lock_required = 0;
	for(uint8_t counter  = 0; counter < 6; counter ++){
		test[counter] = lock_file_name[counter];
	}
	test[5] = 0x00;
	strcpy(cmp, "/dev/");
	cmp[5] = 0x00;
	comparacion = strcmp(cmp, test);
	if(comparacion == 0){
		lock_required = 1;
	}
	/***************************test lock file**************************/

	/***************************test cipher code file**************************/
	if(cipher_code == 1){
		if(cipher_code_file_name[0] == 0x2d){
			printf("%s is an invalid cipher-code.\n", cipher_code_file_name);
			exit(101);
		}
		cipher_code_required = 0;
		for(uint8_t counter  = 0; counter < 6; counter ++){
			test[counter] = cipher_code_file_name[counter];
		}
		test[5] = 0x00;
		strcpy(cmp, "/dev/");
		cmp[5] = 0x00;
		comparacion = strcmp(cmp, test);
		if(comparacion == 0){
			cipher_code_required = 1;
		}
	}
	/***************************test cipher code file**************************/

	/***************************input addresses and size****************/
	input_address = 0;
	if(input_required == 1){
		//address
		strcpy(term_args, "-ia");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				arg_number = counter + 1;
				if(arg_number == cargc){
					printf("-ia: ?\n");
					exit(1);
				}
				input_address = atoll(cargv[counter + 1]);
				if(input_address < 1 || input_address > 1000000000000000000){
					printf("-ia: 1 - 1,000,000,000,000,000,000\n");
					exit(1);
				}
			}
		}
		if(not_found == 1){
			printf("please select input address. -ia.\n");
			exit(1);
		}
		//size
		strcpy(term_args, "-is");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				arg_number = counter + 1;
				if(arg_number == cargc){
					printf("-is: ?\n");
					exit(1);
				}
				input_size = atoll(cargv[counter + 1]);
				if(input_size < 1 || input_size > 1000000000000000000){
					printf("-is: 1 - 1,000,000,000,000,000,000\n");
					exit(1);
				}
			}
		}
		if(not_found == 1){
			printf("please select input size. -is.\n");
			exit(1);
		}
	}
	/***************************input addresses and size****************/

	/***************************output addresses and size****************/
	strcpy(output_mode, "wb");
	output_address = 0;
	if(output_required == 1){
		strcpy(output_mode, "r+b");
		//address
		strcpy(term_args, "-oa");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				arg_number = counter + 1;
				if(arg_number == cargc){
					printf("-oa: ?\n");
					exit(1);
				}
				output_address = atoll(cargv[counter + 1]);
				if(output_address < 1 || output_address > 1000000000000000000){
					printf("-oa: 1 - 1,000,000,000,000,000,000\n");
					exit(1);
				}
			}
		}
		if(not_found == 1){
			printf("please select output address. -oa.\n");
			exit(1);
		}
	}
	/***************************output addresses and size****************/

	/***************************lock addresses and size****************/
	lock_address = 0;
	if(lock_required == 1){
		//address
		strcpy(term_args, "-la");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				arg_number = counter + 1;
				if(arg_number == cargc){
					printf("-la: ?\n");
					exit(1);
				}
				lock_address = atoll(cargv[counter + 1]);
				if(lock_address < 1 || lock_address > 1000000000000000000){
					printf("-la: 1 - 1,000,000,000,000,000,000\n");
					exit(1);
				}
			}
		}
		if(not_found == 1){
			printf("please select cipher-lock-256bit address. -la.\n");
			exit(1);
		}
		//size
		strcpy(term_args, "-ls");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				arg_number = counter + 1;
				if(arg_number == cargc){
					printf("-ls: ?\n");
					exit(1);
				}
				lock_size = atoll(cargv[counter + 1]);
				if(lock_size < 1 || lock_size > 500000){
					printf("-ls: 1 - 500,000\n");
					exit(1);
				}
			}
		}
		if(not_found == 1){
			lock_size = 1;
		}
		lock_size = (lock_size * (32 * (2 + (2 * 8)))) + (65536 * 2) + 32;
	}
	/***************************lock addresses and size****************/

	/***************************cipher code addresses and size****************/
	cipher_code_address = 0;
	if(cipher_code_required == 1){
		//address
		strcpy(term_args, "-cca");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				arg_number = counter + 1;
				if(arg_number == cargc){
					printf("-cca: ?\n");
					exit(1);
				}
				cipher_code_address = atoll(cargv[counter + 1]);
				if(cipher_code_address < 1 || cipher_code_address > 1000000000000000000){
					printf("-cca: 1 - 1,000,000,000,000,000,000\n");
					exit(1);
				}
			}
		}
		if(not_found == 1){
			printf("please select cipher-code address. -cca.\n");
			exit(1);
		}
		//size
		strcpy(term_args, "-ccs");
		not_found = 1;
		for(uint8_t counter = 0; counter < cargc; counter ++){
			comparacion = strcmp(term_args, cargv[counter]);
			if(comparacion == 0){
				not_found = 0;
				arg_number = counter + 1;
				if(arg_number == cargc){
					printf("-ccs: ?\n");
					exit(1);
				}
				cipher_code_size = atoll(cargv[counter + 1]);
				if(cipher_code_size < 300 || cipher_code_size > 500000){
					printf("-ccs: 300 - 500,000.\n");
					exit(1);
				}
			}
		}
		if(not_found == 1){
			printf("please select cipher-code size. -ccs.\n");
			exit(222);
		}
	}
	/***************************cipher code addresses and size****************/
}
