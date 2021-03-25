uint8_t* readLock(uint8_t *cells8, uint16_t *cells16, uint64_t *cells64, char **strg, char **cargv){
	/*file open*/
	FILE* lockfile;
	lockfile = fopen(lock_file_name,"rb");
	if(lockfile == NULL){
		printf("couldn't open %s.\n", lock_file_name);
		exit(1);
	}
	srand(time(NULL));
	/*file open*/

	/*lock size*/
	lock_file_size = lock_size;
	if(lock_required == 0){
		fseek(lockfile, 0, SEEK_END);
		lock_file_size  = ftell(lockfile);
		if(lock_file_size < 131680){
			printf("%s undersize error.\n", lock_file_name);
			fclose(lockfile);
			exit(90);
		}
	}
	if(lock_required == 1){
		fseek(lockfile, 0, SEEK_END);
		drive_size  = ftell(lockfile);
		if(lock_file_size > drive_size - 2){
			printf("cipher-lock-256bit size is %"PRIu64" bytes.  %s size is %"PRIu64" bytes.\n",lock_file_size, lock_file_name , drive_size - 2);
			fclose(lockfile);
			exit(33);
		}
		if(drive_size < lock_address + lock_file_size){
			printf("cipher-lock-256bit boundary error. max start address is %"PRIu64".  %s size is %"PRIu64" bytes.\n", drive_size - lock_file_size, lock_file_name , drive_size);
			fclose(lockfile);
			exit(91);
		}
	}

	uint8_t* lock_block;
	lock_block = (uint8_t*)malloc(lock_file_size * sizeof(uint8_t));
	fseek(lockfile, lock_address, SEEK_SET);
	fread(lock_block, lock_file_size, 1, lockfile);
	fclose(lockfile);
	/*lock size*/

	/*xor lock block*/
	key_counter_reset = lock_file_size / 2;
	for(uint64_t counter = 0; counter < (lock_file_size / 2); counter ++){
		lock_block[key_counter_reset] = lock_block[key_counter_reset] ^ lock_block[counter];
		key_counter_reset ++;
	}
	key_counter_reset = lock_file_size - 1;
	for(uint64_t counter = 0; counter < (lock_file_size / 2); counter ++){
		lock_block[counter] = lock_block[counter] ^ lock_block[key_counter_reset];
		key_counter_reset -- ;
	}
	key_counter_reset = lock_file_size / 2;
	for(uint64_t counter = 0; counter < (lock_file_size / 2); counter ++){
		lock_block[key_counter_reset] = lock_block[key_counter_reset] ^ lock_block[counter];
		key_counter_reset ++;
	}
	key_counter_reset = lock_file_size - 1;
	for(uint64_t counter = 0; counter < (lock_file_size / 2); counter ++){
		lock_block[counter] = lock_block[counter] ^ lock_block[key_counter_reset];
		key_counter_reset -- ;
	}
	/*xor lock block*/

	/*loads sizes from file*/
	for(uint8_t counter = 0; counter < 8; counter ++){
		transpotition_b_size = transpotition_b_size << 8;
		transpotition_b_size = transpotition_b_size & 0xffffffffffffff00;
		transpotition_b_size = transpotition_b_size | lock_block[counter];
	}
	for(uint8_t counter = 8; counter < 16; counter ++){
		key_b_size = key_b_size << 8;
		key_b_size = key_b_size & 0xffffffffffffff00;
		key_b_size = key_b_size | lock_block[counter];
	}
	for(uint8_t counter = 16; counter < 24; counter ++){
		swap_b_size = swap_b_size << 8;
		swap_b_size = swap_b_size & 0xffffffffffffff00;
		swap_b_size = swap_b_size | lock_block[counter];
	}
	for(uint8_t counter = 24; counter < 32; counter ++){
		rotation_b_size = rotation_b_size << 8;
		rotation_b_size = rotation_b_size & 0xffffffffffffff00;
		rotation_b_size = rotation_b_size | lock_block[counter];
	}
	/*loads sizes from file*/

	/*loads sizes to file*/
	kadd = 32 + 65536 + 65536;
	tradd = kadd + key_b_size;
	swpadd = tradd + transpotition_b_size;
	rtfadd = swap_b_size + swpadd;
	if(kadd + key_b_size > lock_file_size){
		printf("%s key sector size error.\n", lock_file_name);
		free(lock_block);
		exit(20);
	}
	if(tradd + transpotition_b_size > lock_file_size){
		printf("%s transpo sector size error.\n", lock_file_name);
		free(lock_block);
		exit(21);
	}
	if(swpadd + swap_b_size > lock_file_size){
		printf("%s swap sector size error.\n", lock_file_name);
		free(lock_block);
		exit(22);
	}
	if(rtfadd + rotation_b_size != lock_file_size){
		printf("%s rotation sector size error.\n", lock_file_name);
		free(lock_block);
		exit(23);
	}
	/*loads sizes to file*/
	return lock_block;
}
