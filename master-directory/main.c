////////////////////////////////////////////////////START//////////////////////////////////
#include "include/header.c"

int main(int argc, char *argv[]){

	/*8bit malloc cells*/
	uint64_t cell8_qty = 256;
	uint8_t *cells8;
	cells8 = (uint8_t*)malloc(cell8_qty * sizeof(uint8_t));
	for(uint16_t clr = 0; clr < cell8_qty; clr ++){
		cells8[clr] = 0;
	}
	/*8bit malloc cells*/
	/*16bit malloc cells*/
	uint64_t cell16_qty = 256;
	uint16_t *cells16;
	cells16 = (uint16_t*)malloc(cell16_qty * sizeof(uint16_t));
	for(uint16_t clr = 0; clr < cell16_qty; clr ++){
		cells16[clr] = 0;
	}
	/*16bit malloc cells*/
	/*64bit malloc cells*/
	uint64_t cell64_qty = 256;
	uint64_t *cells64;
	cells64 = (uint64_t*)malloc(cell64_qty * sizeof(uint64_t));
	for(uint16_t clr = 0; clr < cell64_qty; clr ++){
		cells64[clr] = 0;
	}
	/*64bit malloc cells*/
	/*malloc strings*/
	uint64_t strg_qty = 50, strg_size = 100;
	char **strg = (char**)malloc(strg_qty * sizeof(char*));
	for(uint16_t str = 0; str < strg_qty; str ++){
		strg[str] = (char*)malloc(strg_size * sizeof(char));
	}
	for(uint64_t str_q = 0; str_q < strg_qty; str_q ++){
		for(uint64_t str_z = 0; str_z < strg_size; str_z ++){
			strg[str_q][str_z] = 0;
		}
	}
	/*malloc strings*/
	/*malloc console arguments*/
	uint64_t cargv_size = 100;
	char **cargv = (char**)malloc(argc * sizeof(char*));
	for(uint16_t str = 0; str < argc; str ++){
		cargv[str] = (char*)malloc(cargv_size * sizeof(char));
	}
	for(uint64_t str_q = 0; str_q < argc; str_q ++){
		for(uint64_t str_z = 0; str_z < cargv_size; str_z ++){
			cargv[str_q][str_z] = 0;
		}
	}
	for(uint64_t ctr = 0; ctr < argc; ctr ++){
		strcpy(cargv[ctr], argv[ctr]);
	}
	cargc = argc;
	/*malloc console arguments*/

	/*console input*/
	consoleInput(cells8, cells16, cells64, strg, cargv);
	/*console input*/

	/*generate lock*/
	if(call_gen_lock == 1){
		generateLock(cells8, cells16, cells64, strg, cargv);
	}
	/*generate lock*/

	/*read lock*/
	uint8_t *lock_block = readLock(cells8, cells16, cells64, strg, cargv);
	/*read lock*/

	/*getting key sections size*/
	key_sections = key_b_size / 32;
	transpo_sections = transpotition_b_size / 32;
	/*getting key sections size*/

	/*compare key_shift_location to key size*/
	if(subkey_shift_location > (key_b_size - 3)){
		printf("max -sx for current key is %"PRIu64"\n", key_b_size - 3);
		exit(55);
	}
	if(firstkey_shift_location > (key_b_size - 3)){
		printf("max -fx for current key is %"PRIu64"\n", key_b_size - 3);
		exit(56);
	}
	if(trakey_shift_location > (transpotition_b_size - 3)){
		printf("max -tx for current key is %"PRIu64"\n", transpotition_b_size - 3);
		exit(57);
	}
	/*compare key_shift_location to key size*/

	/*loads key block in array*/
        uint8_t *key_block, *first_key_block;
        key_block = (uint8_t*)malloc(key_b_size * sizeof(uint8_t));
        first_key_block = (uint8_t*)malloc(key_b_size * sizeof(uint8_t));
        for(uint64_t counter = 0; counter < key_b_size; counter ++){
                key_block[counter] = lock_block[counter + kadd];
                first_key_block[counter] = lock_block[counter + kadd];
        }
	/*loads key block in array*/

        /*loads transpotition table generator*/
        uint8_t *transpotition_block;
        transpotition_block = (uint8_t*)malloc(transpotition_b_size * sizeof(uint8_t));
        for(uint64_t counter = 0; counter < transpotition_b_size; counter ++){
                transpotition_block[counter] = lock_block[counter + tradd];
        }
        /*loads transpotition table generator*/

        /*loads rotation block array*/
        uint8_t *rotation_block;
        rotation_block = (uint8_t*)malloc(rotation_b_size * sizeof(uint8_t));
        for(uint64_t counter = 0; counter < rotation_b_size; counter ++){
                rotation_block[counter] = lock_block[counter + rtfadd];
        }
        /*loads rotation block array*/
        /*swap bloxk*/
        uint8_t *swap_block;
        swap_block = (uint8_t*)malloc(swap_b_size * sizeof(uint8_t));
        for(uint64_t counter  = 0; counter < swap_b_size; counter ++){
                swap_block[counter] = lock_block[counter + swpadd];
        }

        if(cipher_code == 1){
                FILE *cipher_code_file;
                cipher_code_file = fopen(cipher_code_file_name, "rb");
                if(cipher_code_file == NULL){
                        printf("couldn't open %s.\n", cipher_code_file_name);
                        free(key_block);
                        free(first_key_block);
                        free(transpotition_block);
                        free(rotation_block);
                        free(swap_block);
                        exit(100);
                }
                uint64_t cipher_code_drive_size, cipher_code_reset;

                if(cipher_code_required == 0){
                        fseek(cipher_code_file, 0, SEEK_END);
                        cipher_code_size = ftell(cipher_code_file);
                        if(cipher_code_size < 100 || cipher_code_size > 500000){
                                printf("%s is %"PRIu64" bytes.  -cc must be 100 to 500,000 bytes long\n", cipher_code_file_name, cipher_code_size);
                                free(key_block);
                                free(first_key_block);
                                free(transpotition_block);
                                free(rotation_block);
                                free(swap_block);
                                exit(100);
                        }
                }
                if(cipher_code_required == 1){
                        fseek(cipher_code_file, 0, SEEK_END);
                        cipher_code_drive_size = ftell(cipher_code_file);

                        if(cipher_code_size > cipher_code_drive_size - 2){
                                printf("cipher-code size is %"PRIu64" bytes.  %s size is %"PRIu64"bytes.\n", cipher_code_size, cipher_code_file_name , cipher_code_drive_size - 2);
                                free(key_block);
                                free(first_key_block);
                                free(transpotition_block);
                                free(rotation_block);
                                free(swap_block);
                                exit(101);
                        }
                        if(cipher_code_drive_size < cipher_code_size + cipher_code_address){
                                printf("cipher_code boundary error.  max start address is %"PRIu64".  %s size is %"PRIu64" bytes.\n", cipher_code_drive_size - cipher_code_size, cipher_code_file_name ,cipher_code_drive_size);
                                free(key_block);
                                free(first_key_block);
                                free(transpotition_block);
                                free(rotation_block);
                                free(swap_block);
                                exit(102);
                        }
                }

                uint8_t *cipher_cb;
                cipher_cb = (uint8_t*)malloc(cipher_code_size * sizeof(uint8_t));
                fseek(cipher_code_file, cipher_code_address, SEEK_SET);
                fread(cipher_cb, cipher_code_size, 1, cipher_code_file);
                /*xor cipher code*/
                for(uint64_t itr = 0; itr < cipher_code_size; itr ++){
                        cipher_cb[0] = cipher_cb[0] ^ cipher_cb[cipher_code_size - 1];
                        for(uint64_t x = 0; x < cipher_code_size - 1; x ++){
                                cipher_cb[x + 1] = cipher_cb[x + 1] ^ cipher_cb[x];
                        }
                }
                /*xor cipher code*/
                cipher_code_reset = 0;
                for(uint64_t ctr = 0; ctr < cipher_code_size; ctr ++){
                        swap_block[cipher_code_reset] = swap_block[cipher_code_reset] ^ cipher_cb[ctr];
                        cipher_code_reset ++;
                        if(cipher_code_reset == swap_b_size){
                                cipher_code_reset = 0;
                        }
                }
                cipher_code_reset = 0;
                for(uint64_t ctr = cipher_code_size; ctr > 0; ctr --){
                        swap_block[cipher_code_reset] = swap_block[cipher_code_reset] ^ cipher_cb[ctr - 1];
                        cipher_code_reset ++;
                        if(cipher_code_reset == swap_b_size){
                                cipher_code_reset = 0;
                        }
                }
                cipher_code_reset = 0;
                for(uint64_t ctr = 0; ctr < cipher_code_size; ctr ++){
                        rotation_block[cipher_code_reset] = rotation_block[cipher_code_reset] ^ cipher_cb[ctr];
                        cipher_code_reset ++;
                        if(cipher_code_reset == rotation_b_size){
                                cipher_code_reset = 0;
                        }
                }
                cipher_code_reset = 0;
                for(uint64_t ctr = cipher_code_size; ctr > 0; ctr --){
                        rotation_block[cipher_code_reset] = rotation_block[cipher_code_reset] ^ cipher_cb[ctr - 1];
                        cipher_code_reset ++;
                        if(cipher_code_reset == rotation_b_size){
                                cipher_code_reset = 0;
                        }
                }

                cipher_code_reset = 0;
                for(uint64_t ctr = 0; ctr < cipher_code_size; ctr ++){
                        key_block[cipher_code_reset] = key_block[cipher_code_reset] ^ cipher_cb[ctr];
                        cipher_code_reset ++;
                        if(cipher_code_reset == key_b_size){
                                cipher_code_reset = 0;
                        }
                }
                cipher_code_reset = 0;
                for(uint64_t ctr = 0; ctr < cipher_code_size; ctr ++){
                        transpotition_block[cipher_code_reset] = transpotition_block[cipher_code_reset] ^ cipher_cb[ctr];
                        cipher_code_reset ++;
                        if(cipher_code_reset == transpotition_b_size){
                                cipher_code_reset = 0;
                        }
                }
                for(uint64_t ctr = 0; ctr < key_b_size; ctr ++){
                        first_key_block[ctr] = key_block[ctr];
                }
                free(cipher_cb);
                fclose(cipher_code_file);
        }
        /*swap block*/


        /********************first swap*************/
        uint8_t *fswap_block;
        fswap_block = (uint8_t*)malloc(swap_b_size * sizeof(uint8_t));
        for(uint64_t counter  = 0; counter < swap_b_size; counter ++){
                fswap_block[counter] = swap_block[counter];
        }
        /********************first swap*************/

        /*transpotition swap block*/
        tswap_b_size = swap_b_size;
        uint8_t *transpo_swap_block;
        transpo_swap_block = (uint8_t*)malloc(tswap_b_size * sizeof(uint8_t));
        for(uint64_t counter  = 0; counter < tswap_b_size; counter ++){
                transpo_swap_block[counter] = swap_block[counter] ^ rotation_block[counter];
        }
        /*transpotition swap block*/

        /*rotors array*/
        uint8_t** rotor;
        rotor = (uint8_t**)malloc(256 * sizeof(uint8_t*));
        for(uint16_t counter = 0; counter < 256; counter ++){
                rotor[counter] = (uint8_t*)malloc(256 * sizeof(uint8_t));
        }
        /*rotors array*/

        /*reverse rotors array*/
        uint8_t** rev_rotor;
        rev_rotor = (uint8_t**)malloc(256 * sizeof(uint8_t*));
        for(uint16_t counter = 0; counter < 256; counter ++){
                rev_rotor[counter] = (uint8_t*)malloc(256 * sizeof(uint8_t));
        }
        /*reverse rotors array*/

        /*transpotition table generation*/
        uint8_t** transpo;
        transpo = (uint8_t**)malloc(256 * sizeof(uint8_t*));
        for(uint16_t counter = 0; counter < 256; counter ++){
                transpo[counter] = (uint8_t*)malloc(256 * sizeof(uint8_t));
        }
        /*transpotition table generation*/

        /*reverse transpotition table generation*/
        uint8_t** rev_transpo;
        rev_transpo = (uint8_t**)malloc(256 * sizeof(uint8_t*));
        for(uint16_t counter = 0; counter < 256; counter ++){
                rev_transpo[counter] = (uint8_t*)malloc(256 * sizeof(uint8_t));
        }
        /*reverse transpotition table generation*/

	/*swapTranspoReading*/
	swapTranspoReading(cells8, cells16, cells64, lock_block, rotor, rev_rotor, transpo, rev_transpo);
	/*swapTranspoReading*/

        /*print serial and verify*/
        if(print_serial == 1 || verify_rt == 1){
                if(verify_errors == 0 && duplicate_errors == 0){
                        printf("cipher-lock-256bit verified OK.\n");
                }
                if(verify_errors > 0 || duplicate_errors > 0){
                        printf("cipher-lock-256bit found %"PRIu64" integrity errors. and %"PRIu64" duplicate errors.\n", verify_errors, duplicate_errors);
                }
                exit_code = 0;
                goto __emergency_exit;
        }
        /*print serial and verify*/

/////////////////////////////////////////////////////CIPHERING/////////////////////////////////////////////////////////////////////////////////
	struct timeval tiempo, start, stop;
        if(cipher_option == 1){

                /*input block*/
                uint8_t *input_block;
                input_block = (uint8_t*)malloc(100000001 * sizeof(uint8_t));
                /*input block*/

                //////////////////////////////////////////////////////*console input txt*///////////////////////////////////
                if(console_input == 1){
                        /*read from console*/
                        for(uint64_t counter  = 0; counter < 100000001; counter ++){
                                input_block[counter] = 0;
                        }

                        printf("enter a maximum of 100,000,000 bytes.  close {} to complete\n{");
                        scanf("%[^}]%*c", input_block);

                        //get input size
                        input_file_size = 0;
                        while(input_file_size < 100000000){
                                search_byte = input_block[input_file_size];
                                if(search_byte == 0x00){
                                        break;
                                }
                                input_file_size ++;
                        }

                        input_block[input_file_size] = 0x0a;
                        input_file_size ++;
                        //input_block = realloc(input_block, input_file_size * sizeof(uint8_t));
                        /*read from console*/
                        goto __skip_input_file;
                }
                //////////////////////////////////////////////////////*console input txt*///////////////////////////////////

                /*file pointer*/
                FILE *input_file;
                input_file = fopen(input_file_name, "rb");
                if(input_file == NULL){
                        printf("couldn't open %s.\n", input_file_name);
                        exit_code = 12;
                        goto __emergency_exit;
                }
                /*file pointer*/

                /*getting input file size*/
                input_file_size = input_size;
                if(input_required == 0){
                        fseek(input_file, 0, SEEK_END);
                        input_file_size  = ftell(input_file);
                }
                if(input_required == 1){
                        fseek(input_file, 0, SEEK_END);
                        drive_size = ftell(input_file);

                        if(input_file_size > drive_size - 2){
                                printf("-is is %"PRIu64" bytes.  %s size is %"PRIu64" bytes.\n", input_file_size, input_file_name , drive_size - 2);
                                free(input_block);
                                exit_code = 33;
                                goto __emergency_exit;
                        }
                        if(drive_size < input_file_size + input_address){
                                printf("input boundary error. max start address is %"PRIu64".  %s size is %"PRIu64" bytes.\n", drive_size - input_file_size, input_file_name ,drive_size);
                                free(input_block);
                                exit_code = 21;
                                goto __emergency_exit;
                        }
                }
                /*getting input file size*/

                //////////////////2 GIG LIMIT////////
                if(input_file_size < 1 || input_file_size > 2000000000){
                        printf("%s size is %"PRIu64" bytes.  max cipher size is 2,000,000,000 bytes.\n", input_file_name, input_file_size);
                        free(input_block);
                        fclose(input_file);
                        exit_code = 120;
                        goto __emergency_exit;
                }
                //////////////////2 GIG LIMIT///////

                /*ckecks print on screen size*/
                if(input_file_size > 10000 && print_option == 1){
                        printf("output too large to print on screen. please save to file.\n");
                        free(input_block);
                        fclose(input_file);
                        exit_code = 11;
                        goto __emergency_exit;
                }
                /*ckecks print on screen size*/

                /*loads file in array*/
                fseek(input_file, input_address, SEEK_SET);
                free(input_block);
                input_block = (uint8_t*)malloc(input_file_size * sizeof(uint8_t));
                fread(input_block, input_file_size, 1, input_file);
                fclose(input_file);
                /*loads file in array*/


__skip_input_file:

                /*print*/
                gettimeofday(&start, NULL);
                /*print*/

                /*xor file with key*/
                key_counter_reset = key_b_size;
                rotation_counter_reset = 0;
                swap_section_counter = 0;
                for(uint64_t counter = 0; counter < input_file_size; counter ++){
                        if(key_counter_reset >= key_b_size){
                                key_counter_reset = 0;
				/*rotateFirstKey.c*/
				rotateFirstKey(cells8, cells16, cells64, key_block, fswap_block, rotation_block);
				/*rotateFirstKey.c*/
                        }
                        input_block[counter] = input_block[counter] ^ key_block[key_counter_reset];
                        key_counter_reset ++;
                }
                /*xor file with key*/

                /*reset key*/
                for(uint64_t counter = 0; counter < key_b_size; counter ++){
                        key_block[counter] = first_key_block[counter];
                }
                swap_section_counter = 0;
                /*reset key*/

                /*packets*/
                packet_packet = 0;
                packets = input_file_size / 240;
                packet_remainder = input_file_size % 240;
                if(packet_remainder > 0){
                        packet_packet = 1;
                }
                packets_total = packets + packet_packet;
                /*packets*/

                /*packets array*/
                uint8_t** packet_block;
                packet_block = (uint8_t**)malloc(packets_total * sizeof(uint8_t*));
                for(uint64_t counter = 0; counter < packets_total; counter ++){
                        packet_block[counter] = (uint8_t*)malloc(256 * sizeof(uint8_t));
                }
                /*packets array*/

                /*loads file in packet array*/
                file_byte = 0;
                if(packets > 0){
                        for(uint64_t pcounter = 0; pcounter < packets; pcounter ++){
                                for(uint16_t counter = 16; counter < 256; counter ++){
                                        packet_block[pcounter][counter] = input_block[file_byte];
                                        file_byte ++;
                                }
                                //checksum
                                checksum = 0;
                                for(uint16_t counter = 16; counter < 256;  counter ++){
                                        checksum = checksum + packet_block[pcounter][counter];
                                }
                                checksum_multi = 0;
                                for(uint16_t counter = 16; counter < 255;  counter ++){
                                        checksum_multi = checksum_multi + (packet_block[pcounter][counter] * packet_block[pcounter][counter + 1]);
                                }
                                check_less = 0;
                                reverse_packet_counter = 255;
                                for(uint16_t counter = 16; counter < 136;  counter ++){
                                        check_less = check_less + (packet_block[pcounter][counter] * packet_block[pcounter][reverse_packet_counter]);
                                        reverse_packet_counter --;
                                }
                                checksum_multi_rev = 64000;
                                for(uint16_t counter = 16; counter < 256; counter = counter + 2){
                                        checksum_multi_rev = checksum_multi_rev + packet_block[pcounter][counter];
                                        checksum_multi_rev = checksum_multi_rev - packet_block[pcounter][counter + 1];
                                }
                                checksum_total = checksum * checksum_multi * check_less * checksum_multi_rev;
                                //checksum

                                //random
                                gettimeofday(&tiempo, NULL);
                                srandom(time(NULL) * tiempo.tv_sec * tiempo.tv_usec * clock() - packets);
                                for(uint8_t randcounter = 0; randcounter < 8; randcounter ++){
                                        packet_block[pcounter][randcounter] = random() % 0x100;
                                }
                                packet_block[pcounter][3] = 240;
                                //load checksum
                                for(uint8_t ckcounter = 8; ckcounter > 0; ckcounter --){
                                        packet_block[pcounter][ckcounter + 7] = checksum_total & 0x00000000000000ff;
                                        checksum_total = checksum_total >> 8;
                                }
                        }
                }

                if(packet_remainder > 0){
                        /*fills with garbage*/
                        gettimeofday(&tiempo, NULL);
                        srandom(time(NULL) * tiempo.tv_sec * tiempo.tv_usec * clock() + packet_remainder);
                        for(uint16_t counter = 0; counter < 256; counter ++){
                                packet_block[packets_total - 1][counter] = random() % 0x100;
                        }
                        /*fills with garbage*/

                        for(uint16_t counter = 16; counter < packet_remainder + 16; counter ++){
                                packet_block[packets_total - 1][counter] = input_block[file_byte];
                                file_byte ++;
                        }

                        //checksum
                        checksum = 0;
                        for(uint16_t counter = 16; counter < 256;  counter ++){
                                checksum = checksum + packet_block[packets_total - 1][counter];
                        }
                        checksum_multi = 0;
                        for(uint16_t counter = 16; counter < 255;  counter ++){
                                checksum_multi = checksum_multi + (packet_block[packets_total - 1][counter] * packet_block[packets_total - 1][counter + 1]);
                        }
                        check_less = 0;
                        reverse_packet_counter = 255;
                        for(uint16_t counter = 16; counter < 136;  counter ++){
                                check_less = check_less + (packet_block[packets_total - 1][counter] * packet_block[packets_total - 1][reverse_packet_counter]);
                                reverse_packet_counter --;
                        }
                        checksum_multi_rev = 64000;
                        for(uint16_t counter = 16; counter < 256; counter = counter + 2){
                                checksum_multi_rev = checksum_multi_rev + packet_block[packets_total - 1][counter];
                                checksum_multi_rev = checksum_multi_rev - packet_block[packets_total - 1][counter + 1];
                        }
                        checksum_total = checksum * checksum_multi * check_less * checksum_multi_rev;
                        //checksum

                        packet_block[packets_total - 1][3] = packet_remainder;
                        //load checksum
                        for(uint8_t ckcounter = 8; ckcounter > 0; ckcounter --){
                                packet_block[packets_total - 1][ckcounter + 7] = checksum_total & 0x00000000000000ff;
                                checksum_total = checksum_total >> 8;
                        }

                }
                /*loads file in packet array*/

                /*output block includding padd*/
                uint8_t *output_block;
                output_block = (uint8_t*)malloc((packets_total * 256) * sizeof(uint8_t));
                /*output block includding padd*/

                /******************************* ALGORITHM ****************************************/
                if(multi_rotation == 1){
                        /*substitution*/ //single_byte_sub = 0, multibyte_sub = 0;
                        rotor_number = 0;
                        search_byte = 0;
                        rotation_counter_reset = 0;
                        swap_section_counter = 0;
                        ciphered_txt_byte = rotation_block[rotation_b_size - 1];
                        for(uint64_t round_counter = 0; round_counter < packets_total; round_counter ++){
                                for(uint16_t xcounter = 0; xcounter < 255; xcounter ++){
                                        packet_block[round_counter][xcounter + 1] = packet_block[round_counter][xcounter] ^ packet_block[round_counter][xcounter + 1];
                                }
                                for(uint16_t pcounter = 0; pcounter < 256; pcounter ++){
					/*rotateSwapKey.c*/
					rotateSwapKey(cells8, cells16, cells64, key_block, swap_block, rotation_block);
					/*rotateSwapKey.c*/
                                        for(uint64_t counter  = 0; counter < key_b_size; counter ++){
                                                search_byte = packet_block[round_counter][pcounter];
                                                rotor_number = key_block[counter];
                                                packet_block[round_counter][pcounter] = rotor[rotor_number][search_byte];
                                        }
                                        ciphered_txt_byte = packet_block[round_counter][pcounter];
                                }
                        }
                        /*substitution*/
                        /*transpotitioning*/
                        uint8_t *temp_transpo;
                        temp_transpo = (uint8_t*)malloc(256 * sizeof(uint8_t));
                        rotation_counter_reset = 0;
                        swap_section_counter = 0;
                        ciphered_txt_byte = rotation_block[rotation_b_size - 2];
                        for(uint64_t pcounter = 0; pcounter < packets_total; pcounter ++){
				/*rotateTranspoKey.c*/
				rotateTranspoKey(cells8, cells16, cells64, transpotition_block, transpo_swap_block, rotation_block);
				/*rotateTranspoKey.c*/
                                for(uint64_t tcounter = 0; tcounter < transpotition_b_size; tcounter ++){
                                        /*swapp*/
                                        tmp_byte = packet_block[pcounter][0];
                                        for(uint16_t scounter = 0; scounter < 255; scounter ++){
                                                packet_block[pcounter][scounter] = packet_block[pcounter][scounter + 1];
                                        }
                                        packet_block[pcounter][255] = tmp_byte;
                                        for(uint8_t counter = 0; counter < 16; counter ++){
                                                packet_block[pcounter][counter] = packet_block[pcounter][counter] ^ packet_block[pcounter][counter + 16];
                                                packet_block[pcounter][counter + 16] = packet_block[pcounter][counter + 16] ^ packet_block[pcounter][counter + 32];
                                                packet_block[pcounter][counter + 32] = packet_block[pcounter][counter + 32] ^ packet_block[pcounter][counter + 48];
                                                packet_block[pcounter][counter + 48] = packet_block[pcounter][counter + 48] ^ packet_block[pcounter][counter + 64];
                                                packet_block[pcounter][counter + 64] = packet_block[pcounter][counter + 64] ^ packet_block[pcounter][counter + 80];
                                                packet_block[pcounter][counter + 80] = packet_block[pcounter][counter + 80] ^ packet_block[pcounter][counter + 96];
                                                packet_block[pcounter][counter + 96] = packet_block[pcounter][counter + 96] ^ packet_block[pcounter][counter + 112];
                                                packet_block[pcounter][counter + 112] = packet_block[pcounter][counter + 112] ^ packet_block[pcounter][counter + 128];
                                                packet_block[pcounter][counter + 128] = packet_block[pcounter][counter + 128] ^ packet_block[pcounter][counter + 144];
                                                packet_block[pcounter][counter + 144] = packet_block[pcounter][counter + 144] ^ packet_block[pcounter][counter + 160];
                                                packet_block[pcounter][counter + 160] = packet_block[pcounter][counter + 160] ^ packet_block[pcounter][counter + 176];
                                                packet_block[pcounter][counter + 176] = packet_block[pcounter][counter + 176] ^ packet_block[pcounter][counter + 192];
                                                packet_block[pcounter][counter + 192] = packet_block[pcounter][counter + 192] ^ packet_block[pcounter][counter + 208];
                                                packet_block[pcounter][counter + 208] = packet_block[pcounter][counter + 208] ^ packet_block[pcounter][counter + 224];
                                                packet_block[pcounter][counter + 224] = packet_block[pcounter][counter + 224] ^ packet_block[pcounter][counter + 240];
                                                packet_block[pcounter][counter + 240] = packet_block[pcounter][counter + 240] ^ packet_block[pcounter][counter];
                                        }
                                        /*swapp*/
                                        for(uint16_t counter  = 0; counter < 256; counter ++){
                                                temp_transpo[counter] = packet_block[pcounter][counter];
                                        }
                                        transpo_select = transpotition_block[tcounter];
                                        for(uint16_t counter  = 0; counter < 256; counter ++){
                                                packet_block[pcounter][counter] = temp_transpo[transpo[transpo_select][counter]];
                                        }
                                }
                                ciphered_txt_byte = packet_block[pcounter][ packet_block[pcounter][0] ];
                        }
                        /*transpotitioning*/
                free(temp_transpo);
                }
                if(single_rotation == 1){
                        /*substitution*/ //single_byte_sub = 0, multibyte_sub = 0;
                        rotor_number = 0;
                        search_byte = 0;
                        rotation_counter_reset = 0;
                        swap_section_counter = 0;
                        ciphered_txt_byte = rotation_block[rotation_b_size - 1];
                        for(uint64_t pcounter = 0; pcounter < packets_total; pcounter ++){
				/*rotateSwapKey.c*/
				rotateSwapKey(cells8, cells16, cells64, key_block, swap_block, rotation_block);
				/*rotateSwapKey.c*/
                                for(uint64_t tcounter = 0; tcounter < key_b_size; tcounter ++){
                                        /*swapp*/
                                        for(uint16_t scounter = 0; scounter < 255; scounter ++){
                                                packet_block[pcounter][scounter + 1] = packet_block[pcounter][scounter + 1] ^ packet_block[pcounter][scounter];
                                        }
                                        for(uint8_t counter = 0; counter < 16; counter ++){
                                                packet_block[pcounter][counter] = packet_block[pcounter][counter] ^ packet_block[pcounter][counter + 16];
                                                packet_block[pcounter][counter + 16] = packet_block[pcounter][counter + 16] ^ packet_block[pcounter][counter + 32];
                                                packet_block[pcounter][counter + 32] = packet_block[pcounter][counter + 32] ^ packet_block[pcounter][counter + 48];
                                                packet_block[pcounter][counter + 48] = packet_block[pcounter][counter + 48] ^ packet_block[pcounter][counter + 64];
                                                packet_block[pcounter][counter + 64] = packet_block[pcounter][counter + 64] ^ packet_block[pcounter][counter + 80];
                                                packet_block[pcounter][counter + 80] = packet_block[pcounter][counter + 80] ^ packet_block[pcounter][counter + 96];
                                                packet_block[pcounter][counter + 96] = packet_block[pcounter][counter + 96] ^ packet_block[pcounter][counter + 112];
                                                packet_block[pcounter][counter + 112] = packet_block[pcounter][counter + 112] ^ packet_block[pcounter][counter + 128];
                                                packet_block[pcounter][counter + 128] = packet_block[pcounter][counter + 128] ^ packet_block[pcounter][counter + 144];
                                                packet_block[pcounter][counter + 144] = packet_block[pcounter][counter + 144] ^ packet_block[pcounter][counter + 160];
                                                packet_block[pcounter][counter + 160] = packet_block[pcounter][counter + 160] ^ packet_block[pcounter][counter + 176];
                                                packet_block[pcounter][counter + 176] = packet_block[pcounter][counter + 176] ^ packet_block[pcounter][counter + 192];
                                                packet_block[pcounter][counter + 192] = packet_block[pcounter][counter + 192] ^ packet_block[pcounter][counter + 208];
                                                packet_block[pcounter][counter + 208] = packet_block[pcounter][counter + 208] ^ packet_block[pcounter][counter + 224];
                                                packet_block[pcounter][counter + 224] = packet_block[pcounter][counter + 224] ^ packet_block[pcounter][counter + 240];
                                                packet_block[pcounter][counter + 240] = packet_block[pcounter][counter + 240] ^ packet_block[pcounter][counter];
                                        }
                                        /*swapp*/
                                        rotor_number = key_block[tcounter];
                                        for(uint16_t ctr = 0; ctr < 256; ctr ++){
                                                search_byte = packet_block[pcounter][ctr];
                                                packet_block[pcounter][ctr] = rotor[rotor_number][search_byte];
                                        }
                                }
                                ciphered_txt_byte = packet_block[pcounter][ packet_block[pcounter][0] ];
                        }
                        /*substitution*/
                        /*transpotitioning*/
                        uint8_t *temp_transpo;
                        temp_transpo = (uint8_t*)malloc(256 * sizeof(uint8_t));
                        rotation_counter_reset = 0;
                        swap_section_counter = 0;
                        ciphered_txt_byte = rotation_block[rotation_b_size - 2];
                        for(uint64_t pcounter = 0; pcounter < packets_total; pcounter ++){
				/*rotateTranspoKey.c*/
				rotateTranspoKey(cells8, cells16, cells64, transpotition_block, transpo_swap_block, rotation_block);
				/*rotateTranspoKey.c*/
                                for(uint64_t tcounter = 0; tcounter < transpotition_b_size; tcounter ++){
                                        /*swapp*/
                                        tmp_byte = packet_block[pcounter][0];
                                        for(uint16_t scounter = 0; scounter < 255; scounter ++){
                                                packet_block[pcounter][scounter] = packet_block[pcounter][scounter + 1];
                                        }
                                        packet_block[pcounter][255] = tmp_byte;
                                        for(uint8_t counter = 0; counter < 16; counter ++){
                                                packet_block[pcounter][counter] = packet_block[pcounter][counter] ^ packet_block[pcounter][counter + 16];
                                                packet_block[pcounter][counter + 16] = packet_block[pcounter][counter + 16] ^ packet_block[pcounter][counter + 32];
                                                packet_block[pcounter][counter + 32] = packet_block[pcounter][counter + 32] ^ packet_block[pcounter][counter + 48];
                                                packet_block[pcounter][counter + 48] = packet_block[pcounter][counter + 48] ^ packet_block[pcounter][counter + 64];
                                                packet_block[pcounter][counter + 64] = packet_block[pcounter][counter + 64] ^ packet_block[pcounter][counter + 80];
                                                packet_block[pcounter][counter + 80] = packet_block[pcounter][counter + 80] ^ packet_block[pcounter][counter + 96];
                                                packet_block[pcounter][counter + 96] = packet_block[pcounter][counter + 96] ^ packet_block[pcounter][counter + 112];
                                                packet_block[pcounter][counter + 112] = packet_block[pcounter][counter + 112] ^ packet_block[pcounter][counter + 128];
                                                packet_block[pcounter][counter + 128] = packet_block[pcounter][counter + 128] ^ packet_block[pcounter][counter + 144];
                                                packet_block[pcounter][counter + 144] = packet_block[pcounter][counter + 144] ^ packet_block[pcounter][counter + 160];
                                                packet_block[pcounter][counter + 160] = packet_block[pcounter][counter + 160] ^ packet_block[pcounter][counter + 176];
                                                packet_block[pcounter][counter + 176] = packet_block[pcounter][counter + 176] ^ packet_block[pcounter][counter + 192];
                                                packet_block[pcounter][counter + 192] = packet_block[pcounter][counter + 192] ^ packet_block[pcounter][counter + 208];
                                                packet_block[pcounter][counter + 208] = packet_block[pcounter][counter + 208] ^ packet_block[pcounter][counter + 224];
                                                packet_block[pcounter][counter + 224] = packet_block[pcounter][counter + 224] ^ packet_block[pcounter][counter + 240];
                                                packet_block[pcounter][counter + 240] = packet_block[pcounter][counter + 240] ^ packet_block[pcounter][counter];
                                        }
                                        /*swapp*/
                                        for(uint16_t counter  = 0; counter < 256; counter ++){
                                                temp_transpo[counter] = packet_block[pcounter][counter];
                                        }
                                        transpo_select = transpotition_block[tcounter];
                                        for(uint16_t counter  = 0; counter < 256; counter ++){
                                                packet_block[pcounter][counter] = temp_transpo[transpo[transpo_select][counter]];
                                        }
                                }
                                ciphered_txt_byte = packet_block[pcounter][ packet_block[pcounter][0] ];
                        }
                        /*transpotitioning*/
                free(temp_transpo);
                }
                if(multimixed_rotation == 1){
                        /*substitution*/ //single_byte_sub = 0, multibyte_sub = 0;
                        //temp var saving
                        uint64_t trotation = 0, tswap = 0;
                        uint64_t srotation = 0, sswap = 0;
                        uint8_t tciphered, sciphered;
                        //temp var saving

                        //swapp
                        rotor_number = 0;
                        search_byte = 0;
                        rotation_counter_reset = 0;
                        swap_section_counter = 0;
                        sciphered = rotation_block[rotation_b_size - 1];
                        //swapp
                        //transp
                        uint8_t *temp_transpo;
                        temp_transpo = (uint8_t*)malloc(256 * sizeof(uint8_t));
                        rotation_counter_reset = 0;
                        swap_section_counter = 0;
                        tciphered = rotation_block[rotation_b_size - 2];
                        //transp

                        for(uint64_t pcounter = 0; pcounter < packets_total; pcounter ++){
                                for(uint16_t turns = 0; turns < multimixed_turns; turns ++){
                                        ciphered_txt_byte = sciphered;
                                        rotation_counter_reset = srotation;
                                        swap_section_counter = sswap;

					/*rotateSwapKey.c*/
					rotateSwapKey(cells8, cells16, cells64, key_block, swap_block, rotation_block);
					/*rotateSwapKey.c*/
                                        srotation = rotation_counter_reset;
                                        sswap = swap_section_counter;

                                        ciphered_txt_byte = tciphered;
                                        rotation_counter_reset = trotation;
                                        swap_section_counter = tswap;

					/*rotateTranspoKey.c*/
					rotateTranspoKey(cells8, cells16, cells64, transpotition_block, transpo_swap_block, rotation_block);
					/*rotateTranspoKey.c*/
                                        trotation = rotation_counter_reset;
                                        tswap = swap_section_counter;

                                        for(uint64_t tcounter = 0; tcounter < key_b_size; tcounter ++){
                                                /*substitution*/
                                                /*swapp*/
                                                for(uint16_t scounter = 0; scounter < 255; scounter ++){
                                                        packet_block[pcounter][scounter + 1] = packet_block[pcounter][scounter + 1] ^ packet_block[pcounter][scounter];
                                                }
                                                for(uint8_t counter = 0; counter < 16; counter ++){
                                                        packet_block[pcounter][counter] = packet_block[pcounter][counter] ^ packet_block[pcounter][counter + 16];
                                                        packet_block[pcounter][counter + 16] = packet_block[pcounter][counter + 16] ^ packet_block[pcounter][counter + 32];
                                                        packet_block[pcounter][counter + 32] = packet_block[pcounter][counter + 32] ^ packet_block[pcounter][counter + 48];
                                                        packet_block[pcounter][counter + 48] = packet_block[pcounter][counter + 48] ^ packet_block[pcounter][counter + 64];
                                                        packet_block[pcounter][counter + 64] = packet_block[pcounter][counter + 64] ^ packet_block[pcounter][counter + 80];
                                                        packet_block[pcounter][counter + 80] = packet_block[pcounter][counter + 80] ^ packet_block[pcounter][counter + 96];
                                                        packet_block[pcounter][counter + 96] = packet_block[pcounter][counter + 96] ^ packet_block[pcounter][counter + 112];
                                                        packet_block[pcounter][counter + 112] = packet_block[pcounter][counter + 112] ^ packet_block[pcounter][counter + 128];
                                                        packet_block[pcounter][counter + 128] = packet_block[pcounter][counter + 128] ^ packet_block[pcounter][counter + 144];
                                                        packet_block[pcounter][counter + 144] = packet_block[pcounter][counter + 144] ^ packet_block[pcounter][counter + 160];
                                                        packet_block[pcounter][counter + 160] = packet_block[pcounter][counter + 160] ^ packet_block[pcounter][counter + 176];
                                                        packet_block[pcounter][counter + 176] = packet_block[pcounter][counter + 176] ^ packet_block[pcounter][counter + 192];
                                                        packet_block[pcounter][counter + 192] = packet_block[pcounter][counter + 192] ^ packet_block[pcounter][counter + 208];
                                                        packet_block[pcounter][counter + 208] = packet_block[pcounter][counter + 208] ^ packet_block[pcounter][counter + 224];
                                                        packet_block[pcounter][counter + 224] = packet_block[pcounter][counter + 224] ^ packet_block[pcounter][counter + 240];
                                                        packet_block[pcounter][counter + 240] = packet_block[pcounter][counter + 240] ^ packet_block[pcounter][counter];
                                                }
                                                /*swapp*/
                                                rotor_number = key_block[tcounter];
                                                for(uint16_t ctr = 0; ctr < 256; ctr ++){
                                                        search_byte = packet_block[pcounter][ctr];
                                                        packet_block[pcounter][ctr] = rotor[rotor_number][search_byte];
                                                }
                                                /*substitution*/

                                                /*transpotitioning*/
                                                /*swapp*/
                                                tmp_byte = packet_block[pcounter][0];
                                                for(uint16_t scounter = 0; scounter < 255; scounter ++){
                                                        packet_block[pcounter][scounter] = packet_block[pcounter][scounter + 1];
                                                }
                                                packet_block[pcounter][255] = tmp_byte;
                                                for(uint8_t counter = 0; counter < 16; counter ++){
                                                        packet_block[pcounter][counter] = packet_block[pcounter][counter] ^ packet_block[pcounter][counter + 16];
                                                        packet_block[pcounter][counter + 16] = packet_block[pcounter][counter + 16] ^ packet_block[pcounter][counter + 32];
                                                        packet_block[pcounter][counter + 32] = packet_block[pcounter][counter + 32] ^ packet_block[pcounter][counter + 48];
                                                        packet_block[pcounter][counter + 48] = packet_block[pcounter][counter + 48] ^ packet_block[pcounter][counter + 64];
                                                        packet_block[pcounter][counter + 64] = packet_block[pcounter][counter + 64] ^ packet_block[pcounter][counter + 80];
                                                        packet_block[pcounter][counter + 80] = packet_block[pcounter][counter + 80] ^ packet_block[pcounter][counter + 96];
                                                        packet_block[pcounter][counter + 96] = packet_block[pcounter][counter + 96] ^ packet_block[pcounter][counter + 112];
                                                        packet_block[pcounter][counter + 112] = packet_block[pcounter][counter + 112] ^ packet_block[pcounter][counter + 128];
                                                        packet_block[pcounter][counter + 128] = packet_block[pcounter][counter + 128] ^ packet_block[pcounter][counter + 144];
                                                        packet_block[pcounter][counter + 144] = packet_block[pcounter][counter + 144] ^ packet_block[pcounter][counter + 160];
                                                        packet_block[pcounter][counter + 160] = packet_block[pcounter][counter + 160] ^ packet_block[pcounter][counter + 176];
                                                        packet_block[pcounter][counter + 176] = packet_block[pcounter][counter + 176] ^ packet_block[pcounter][counter + 192];
                                                        packet_block[pcounter][counter + 192] = packet_block[pcounter][counter + 192] ^ packet_block[pcounter][counter + 208];
                                                        packet_block[pcounter][counter + 208] = packet_block[pcounter][counter + 208] ^ packet_block[pcounter][counter + 224];
                                                        packet_block[pcounter][counter + 224] = packet_block[pcounter][counter + 224] ^ packet_block[pcounter][counter + 240];
                                                        packet_block[pcounter][counter + 240] = packet_block[pcounter][counter + 240] ^ packet_block[pcounter][counter];
                                                }
                                                /*swapp*/
                                                for(uint16_t counter  = 0; counter < 256; counter ++){
                                                        temp_transpo[counter] = packet_block[pcounter][counter];
                                                }
                                                transpo_select = transpotition_block[tcounter];
                                                for(uint16_t counter  = 0; counter < 256; counter ++){
                                                        packet_block[pcounter][counter] = temp_transpo[transpo[transpo_select][counter]];
                                                }
                                                /*transpotitioning*/
                                        }
                                }
                                sciphered = packet_block[pcounter][ packet_block[pcounter][0] ];
                                tciphered = packet_block[pcounter][ packet_block[pcounter][255] ];
                        }
                free(temp_transpo);
                }
                /******************************** ALGORITHM ***************************************/
                /*loads packets into output block*/
                file_byte = 0;
                for(uint64_t pcounter = 0; pcounter < packets_total; pcounter ++){
                        for(uint16_t counter = 0; counter < 256; counter ++){
                                output_block[file_byte] = packet_block[pcounter][counter];
                                file_byte ++;
                        }
                }
                /*loads packets into output block*/

                /*****************oblock_shift**************/
                if(oblock_shift > 0){
                        for(uint64_t ctr = 0; ctr < oblock_shift; ctr ++){
                                file_byte = output_block[(packets_total * 256) - 1];
                                for(uint64_t obs = (packets_total * 256) - 1; obs > 0; obs --){
                                        output_block[obs] = output_block[obs - 1];
                                }
                                output_block[0] = file_byte;
                                output_block[0] = output_block[0] ^ output_block[(packets_total * 256) - 1];

                                for(uint64_t counter = packets_total * 256; counter > 1; counter --){
                                        output_block[counter - 2] = output_block[counter - 2] ^ output_block[counter - 1];
                                }
                                output_block[(packets_total * 256) - 1] = output_block[(packets_total * 256) - 1] ^ output_block[0];
                        }
                }
                /*****************oblock_shift**************/

                /*final xor*/
                if(oblock_shift == 0){
                        for(uint64_t counter = packets_total * 256; counter > 1; counter --){
                                output_block[counter - 2] = output_block[counter - 2] ^ output_block[counter - 1];
                        }
                        output_block[(packets_total * 256) - 1] = output_block[(packets_total * 256) - 1] ^ output_block[0];
                }
                /*final xor*/


                /*saving to out file*/
                if(print_option == 0){
                        FILE *output_file;
                        output_file = fopen(output_file_name, output_mode);
                        if(output_file == NULL){
                                printf("couldn't open %s.\n", output_file_name);
                                free(output_block);
                                free(input_block);
                                free(packet_block);
                                exit_code  = 4;
                                goto __emergency_exit;
                        }
                        /*+++++++++checks output size++++++++++++*/
                        if(output_required == 1){
                                fseek(output_file, 0, SEEK_END);
                                drive_size = ftell(output_file);
                                if(drive_size - 2 < output_address + (packets_total * 256)){
                                        printf("output boundary error. max start address is %"PRIu64".  %s size is %"PRIu64" bytes.\n", drive_size - (packets_total * 256), output_file_name ,drive_size);
                                        free(output_block);
                                        free(input_block);
                                        free(packet_block);
                                        fclose(output_file);
                                        exit_code  = 5;
                                        goto __emergency_exit;
                                }
                        }
                        /*+++++++++checks output size++++++++++++*/
                        fseek(output_file, output_address, SEEK_SET);
                        fwrite(output_block, packets_total * 256, 1, output_file);
                        fclose(output_file);
                }
                /*saving to out file*/

                /*printing in screen*/
                if(print_option == 1){
                        if(print_hex == 0 && print_upp_hex == 0){
                        ////////////////////////////////////print-cii////////////////////////////////////
                                uint8_t *table;
                                table = (uint8_t*)malloc(256 * sizeof(uint8_t));

                                table[0x00] = 'A';
                                table[0x01] = 'B';
                                table[0x02] = 'C';
                                table[0x03] = 'D';
                                table[0x04] = 'E';
                                table[0x05] = 'F';
                                table[0x06] = 'G';
                                table[0x07] = 'H';
                                table[0x08] = 'I';
                                table[0x09] = 'J';

                                table[0x0a] = 'K';
                                table[0x0b] = 'L';
                                table[0x0c] = 'M';
                                table[0x0d] = 'N';
                                table[0x0e] = 'O';
                                table[0x0f] = 'P';
                                table[0x10] = 'Q';
                                table[0x11] = 'R';
                                table[0x12] = 'S';
                                table[0x13] = 'T';

                                table[0x14] = 'U';
                                table[0x15] = 'V';
                                table[0x16] = 'W';
                                table[0x17] = 'X';
                                table[0x18] = 'Y';
                                table[0x19] = 'Z';
                                table[0x1a] = 'a';
                                table[0x1b] = 'b';
                                table[0x1c] = 'c';
                                table[0x1d] = 'd';

                                table[0x1e] = 'e';
                                table[0x1f] = 'f';
                                table[0x20] = 'g';
                                table[0x21] = 'h';
                                table[0x22] = 'i';
                                table[0x23] = 'j';
                                table[0x24] = 'k';
                                table[0x25] = 'l';
                                table[0x26] = 'm';
                                table[0x27] = 'n';

                                table[0x28] = 'o';
                                table[0x29] = 'p';
                                table[0x2a] = 'q';
                                table[0x2b] = 'r';
                                table[0x2c] = 's';
                                table[0x2d] = 't';
                                table[0x2e] = 'u';
                                table[0x2f] = 'v';
                                table[0x30] = 'w';
                                table[0x31] = 'x';

                                table[0x32] = 'y';
                                table[0x33] = 'z';
                                table[0x34] = '0';
                                table[0x35] = '1';
                                table[0x36] = '2';
                                table[0x37] = '3';
                                table[0x38] = '4';
                                table[0x39] = '5';
                                table[0x3a] = '6';
                                table[0x3b] = '7';

                                table[0x3c] = '8';
                                table[0x3d] = '9';
                                table[0x3e] = '_';
                                table[0x3f] = '-';

                                //DEVISION
                                uint64_t print_packets;
                                print_packets = (packets_total * 256) / 3;

                                uint8_t print_remainder, extra_packet = 0;
                                print_remainder = (packets_total * 256) % 3;

                                if(print_remainder > 0){
                                        extra_packet = 1;
                                }
                                //DEVISION

                                uint32_t printer;
                                uint8_t cell0, cell1, cell2, cell3;
                                uint8_t reset_reset = 0;
                                uint64_t file_counter = 0;
                                /*printing*/
                                for(uint64_t counter = 0; counter < print_packets; counter ++){
                                        if(reset_reset == (column_number / 4)){
                                                reset_reset = 0;
                                                printf("\n");
                                        }
                                        printer = 0;
                                        printer = printer | output_block[file_counter];
                                        printer = printer << 8;
                                        file_counter ++;
                                        printer = printer | output_block[file_counter];
                                        printer = printer << 8;
                                        file_counter ++;
                                        printer = printer | output_block[file_counter];
                                        file_counter ++;


                                        cell3 = printer & 0x3f;
                                        printer = printer >> 6;
                                        cell2 = printer & 0x3f;
                                        printer = printer >> 6;
                                        cell1 = printer & 0x3f;
                                        printer = printer >> 6;
                                        cell0 = printer & 0x3f;
                                        printf("%c", table[cell0]);
                                        printf("%c", table[cell1]);
                                        printf("%c", table[cell2]);
                                        printf("%c", table[cell3]);

                                        reset_reset ++;
                                }

                                if(reset_reset == (column_number / 4)){
                                        reset_reset = 0;
                                        printf("\n");
                                }

                                gettimeofday(&tiempo, NULL);
                                srandom(time(NULL) * tiempo.tv_sec * tiempo.tv_usec * clock());
                                if(extra_packet == 1){
                                        if(print_remainder == 1){
                                                printer = 0;
                                                printer = printer | output_block[file_counter];
                                                printer = printer << 8;
                                                printer = printer | random() % 0x100;
                                                printer = printer << 8;
                                                printer = printer | random() % 0x100;

                                                cell3 = printer & 0x3f;
                                                printer = printer >> 6;
                                                cell2 = printer & 0x3f;
                                                printer = printer >> 6;
                                                cell1 = printer & 0x3f;
                                                printer = printer >> 6;
                                                cell0 = printer & 0x3f;
                                                printf("%c", table[cell0]);
                                                printf("%c", table[cell1]);
                                                printf("%c", table[cell2]);
                                                printf("%c", table[cell3]);

                                                reset_reset ++;

                                                if(reset_reset == (column_number / 4)){
                                                        reset_reset = 0;
                                                        printf("\n");
                                                }

                                                printf("=");
                                        }
                                        if(print_remainder == 2){
                                                printer = 0;
                                                printer = printer | output_block[file_counter];
                                                printer = printer << 8;
                                                file_counter ++;
                                                printer = printer | output_block[file_counter];
                                                printer = printer << 8;
                                                printer = printer | random() % 0x100;

                                                cell3 = printer & 0x3f;
                                                printer = printer >> 6;
                                                cell2 = printer & 0x3f;
                                                printer = printer >> 6;
                                                cell1 = printer & 0x3f;
                                                printer = printer >> 6;
                                                cell0 = printer & 0x3f;
                                                printf("%c", table[cell0]);
                                                printf("%c", table[cell1]);
                                                printf("%c", table[cell2]);
                                                printf("%c", table[cell3]);

                                                reset_reset ++;

                                                if(reset_reset == (column_number / 4)){
                                                        reset_reset = 0;
                                                        printf("\n");
                                                }

                                                printf("==");

                                        }
                                }
                                printf("\n");
                                /*printing*/
                        free(table);
                        }
                        ////////////////////////////////////print-cii////////////////////////////////////
                        ////////////////////////////////////print-hex////////////////////////////////////
                        if(print_hex == 1){
                                /*prints to screen*/
                                blocks_32_counter = 0;
                                for(uint64_t counter = 0; counter < (packets_total * 256); counter ++){
                                        if(blocks_32_counter == (column_number / 2)){
                                                ////////////
                                                printf("   ");
                                                for(uint32_t pasc = 0; pasc < (column_number / 2); pasc ++){
                                                        if(output_block[counter - blocks_32_counter + pasc] < 0x20){
                                                                printf(".");
                                                        }
                                                        if(output_block[counter - blocks_32_counter + pasc] > 0x7e){
                                                                printf(".");
                                                        }
                                                        if(output_block[counter - blocks_32_counter + pasc] > 0x1f &&  output_block[counter - blocks_32_counter + pasc] < 0x7f){
                                                                printf("%c", output_block[counter - blocks_32_counter + pasc]);
                                                        }
                                                }
                                                ////////////
                                                printf("\n");
                                                blocks_32_counter = 0;
                                        }
                                        printf("%02"PRIx8, output_block[counter]);
                                        blocks_32_counter ++;
                                }
                                //////print ascii//////
                                if(blocks_32_counter <= (column_number / 2)){
                                        printf("   ");
                                        for(uint32_t pasc = 0; pasc < (column_number / 2) -  blocks_32_counter; pasc ++){
                                                printf("  ");
                                        }
                                        for(uint32_t pasc = blocks_32_counter; pasc > 0; pasc --){
                                                if(output_block[(packets_total * 256) - pasc] < 0x20){
                                                        printf(".");
                                                }
                                                if(output_block[(packets_total * 256) - pasc] > 0x7e){
                                                        printf(".");
                                                }
                                                if(output_block[(packets_total * 256) - pasc] > 0x1f &&  output_block[(packets_total * 256) - pasc] < 0x7f){
                                                        printf("%c", output_block[(packets_total * 256) - pasc]);
                                                }
                                        }
                                }
                                //////print ascii//////
                                printf("\n");
                                /*prints to screen*/
                        }
                        if(print_upp_hex == 1){
                                /*prints to screen*/
                                blocks_32_counter = 0;
                                for(uint64_t counter = 0; counter < (packets_total * 256); counter ++){
                                        if(blocks_32_counter == (column_number / 2)){
                                                ////////////
                                                printf("   ");
                                                for(uint32_t pasc = 0; pasc < (column_number / 2); pasc ++){
                                                        if(output_block[counter - blocks_32_counter + pasc] < 0x20){
                                                                printf(".");
                                                        }
                                                        if(output_block[counter - blocks_32_counter + pasc] > 0x7e){
                                                                printf(".");
                                                        }
                                                        if(output_block[counter - blocks_32_counter + pasc] > 0x1f &&  output_block[counter - blocks_32_counter + pasc] < 0x7f){
                                                                printf("%c", output_block[counter - blocks_32_counter + pasc]);
                                                        }
                                                }
                                                ////////////
                                                printf("\n");
                                                blocks_32_counter = 0;
                                        }
                                        printf("%02"PRIX8, output_block[counter]);
                                        blocks_32_counter ++;
                                }
                                //////print ascii//////
                                if(blocks_32_counter <= (column_number / 2)){
                                        printf("   ");
                                        for(uint32_t pasc = 0; pasc < (column_number / 2) -  blocks_32_counter; pasc ++){
                                                printf("  ");
                                        }
                                        for(uint32_t pasc = blocks_32_counter; pasc > 0; pasc --){
                                                if(output_block[(packets_total * 256) - pasc] < 0x20){
                                                        printf(".");
                                                }
                                                if(output_block[(packets_total * 256) - pasc] > 0x7e){
                                                        printf(".");
                                                }
                                                if(output_block[(packets_total * 256) - pasc] > 0x1f &&  output_block[(packets_total * 256) - pasc] < 0x7f){
                                                        printf("%c", output_block[(packets_total * 256) - pasc]);
                                                }
                                        }
                                }
                                //////print ascii//////
                                printf("\n");
                                /*prints to screen*/
                        }
                        ////////////////////////////////////print-hex////////////////////////////////////
                }
                /*printing in screen*/

                /*results if output required*/
                if(output_required == 1){
                        printf("output %"PRIu64" bytes.  addresses: %"PRIu64" - %"PRIu64"\n", packets_total * 256, output_address, output_address + (packets_total * 256) - 1);
                }
                /*results if output required*/

                free(output_block);
                free(packet_block);
                free(input_block);
                exit_code = 0;

        }
/////////////////////////////////////////////////////CIPHERING/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////DECIPHERING/////////////////////////////////////////////////////////////////////////////////
        if(decipher_option == 1){

                /*file pointer*/
                FILE *input_file;
                input_file = fopen(input_file_name, "rb");
                if(input_file == NULL){
                        printf("couldn't open %s.\n", input_file_name);
                        exit_code = 88;
                        goto __emergency_exit;
                }
                /*file pointer*/

                /*getting input file size*/
                input_file_size = input_size;
                if(input_required == 0){
                        fseek(input_file, 0, SEEK_END);
                        input_file_size  = ftell(input_file);
                }
                if(input_required == 1){
                        fseek(input_file, 0, SEEK_END);
                        drive_size = ftell(input_file);
                        if(input_file_size > drive_size - 2){
                                printf("-is is %"PRIu64" bytes.  %s size is %"PRIu64" bytes.\n", input_file_size, input_file_name , drive_size - 2);
                                fclose(input_file);
                                exit_code = 33;
                                goto __emergency_exit;
                        }
                        if(drive_size < input_address + input_file_size){
                                printf("input boundary error. max start address is %"PRIu64".  %s size is %"PRIu64" bytes.\n", drive_size - input_file_size, input_file_name ,drive_size);
                                fclose(input_file);
                                exit_code = 77;
                                goto __emergency_exit;
                        }
                }
                /*getting input file size*/

                //////////////////2.5 GIG LIMIT////////
                if(input_file_size < 1 || input_file_size > 2500000000){
                        printf("%s size is %"PRIu64" bytes.  max decipher size is 2,500,000,000 bytes.\n", input_file_name, input_file_size);
                        fclose(input_file);
                        exit_code = 120;
                        goto __emergency_exit;
                }
                //////////////////2.5 GIG LIMIT///////

                /*ckecks print on screen size*/
                if(input_file_size > 10000 && print_option == 1){
                        printf("output too large to print on screen. please save to file.\n");
                        fclose(input_file);
                        exit_code = 12;
                        goto __emergency_exit;
                }
                /*ckecks print on screen size*/

                /*loads file in array*/
                uint8_t *input_block;
                input_block = (uint8_t*)malloc(input_file_size * sizeof(uint8_t));
                fseek(input_file, input_address, SEEK_SET);
                fread(input_block, input_file_size, 1, input_file);
                fclose(input_file);
                /*loads file in array*/

                /*checking file size*/
                packets = input_file_size / 256;
                packet_remainder = input_file_size % 256;
                if(packet_remainder > 0){
                        printf("decipher file size error. %"PRIu64" bytes.  expected %"PRIu64" || %"PRIu64".\n", input_file_size, input_file_size - packet_remainder, input_file_size - packet_remainder + 256);
                        free(input_block);
                        exit_code = 7;
                        goto __emergency_exit;
                }
                /*checking file size*/


                /*print*/
                gettimeofday(&start, NULL);
                /*print*/

                /*first xor*/
                if(oblock_shift == 0){
                        input_block[input_file_size - 1] = input_block[input_file_size - 1] ^ input_block[0];
                        for(uint64_t counter = 0; counter < input_file_size - 1; counter ++){
                                input_block[counter] = input_block[counter] ^ input_block[counter + 1];
                        }
                }
                /*first xor*/

                /*****************oblock_shift**************/
                if(oblock_shift > 0){
                        for(uint64_t ctr = 0; ctr < oblock_shift; ctr ++){
                                input_block[input_file_size - 1] = input_block[input_file_size - 1] ^ input_block[0];
                                for(uint64_t counter = 0; counter < input_file_size - 1; counter ++){
                                        input_block[counter] = input_block[counter] ^ input_block[counter + 1];
                                }

                                input_block[0] = input_block[0] ^ input_block[input_file_size - 1];
                                file_byte = input_block[0];
                                for(uint64_t obs = 0; obs < input_file_size - 1; obs ++){
                                        input_block[obs] = input_block[obs + 1];
                                }
                                input_block[input_file_size - 1] = file_byte;
                        }
                }
                /*****************oblock_shift**************/

                /*packets array*/
                uint8_t** packet_block;
                packet_block = (uint8_t**)malloc(packets * sizeof(uint8_t*));
                for(uint64_t counter = 0; counter < packets; counter ++){
                        packet_block[counter] = (uint8_t*)malloc(256 * sizeof(uint8_t));
                }
                /*packets array*/

                /*load input file into packets*/
                file_byte = 0;
                for(uint64_t pcounter = 0; pcounter < packets; pcounter ++){
                        for(uint16_t counter = 0; counter < 256; counter ++){
                                packet_block[pcounter][counter] = input_block[file_byte];
                                file_byte ++;
                        }
                }
                /*load input file into packets*/
                /******************************* ALGORITHM ****************************************/
                if(multi_rotation == 1){
                        /*transpotitioning*/
                        uint8_t *temp_transpo;
                        temp_transpo = (uint8_t*)malloc(256 * sizeof(uint8_t));
                        swap_section_counter = 0;
                        rotation_counter_reset = 0;
                        ciphered_txt_byte = rotation_block[rotation_b_size - 2];
                        for(uint64_t pcounter = 0; pcounter < packets; pcounter ++){
				/*rotateTranspoKey.c*/
				rotateTranspoKey(cells8, cells16, cells64, transpotition_block, transpo_swap_block, rotation_block);
				/*rotateTranspoKey.c*/
                                ciphered_txt_byte = packet_block[pcounter][ packet_block[pcounter][0] ];
                                for(uint64_t tcounter = transpotition_b_size; tcounter > 0; tcounter --){
                                        for(uint16_t counter  = 0; counter < 256; counter ++){
                                                temp_transpo[counter] = packet_block[pcounter][counter];
                                        }
                                        transpo_select = transpotition_block[tcounter - 1];
                                        for(uint16_t counter  = 0; counter < 256; counter ++){
                                                packet_block[pcounter][counter] = temp_transpo[rev_transpo[transpo_select][counter]];
                                        }
                                        /*swapp*/
                                        for(uint8_t counter = 0; counter < 16; counter ++){
                                                packet_block[pcounter][counter + 240] = packet_block[pcounter][counter + 240] ^ packet_block[pcounter][counter];
                                                packet_block[pcounter][counter + 224] = packet_block[pcounter][counter + 224] ^ packet_block[pcounter][counter + 240];
                                                packet_block[pcounter][counter + 208] = packet_block[pcounter][counter + 208] ^ packet_block[pcounter][counter + 224];
                                                packet_block[pcounter][counter + 192] = packet_block[pcounter][counter + 192] ^ packet_block[pcounter][counter + 208];
                                                packet_block[pcounter][counter + 176] = packet_block[pcounter][counter + 176] ^ packet_block[pcounter][counter + 192];
                                                packet_block[pcounter][counter + 160] = packet_block[pcounter][counter + 160] ^ packet_block[pcounter][counter + 176];
                                                packet_block[pcounter][counter + 144] = packet_block[pcounter][counter + 144] ^ packet_block[pcounter][counter + 160];
                                                packet_block[pcounter][counter + 128] = packet_block[pcounter][counter + 128] ^ packet_block[pcounter][counter + 144];
                                                packet_block[pcounter][counter + 112] = packet_block[pcounter][counter + 112] ^ packet_block[pcounter][counter + 128];
                                                packet_block[pcounter][counter + 96] = packet_block[pcounter][counter + 96] ^ packet_block[pcounter][counter + 112];
                                                packet_block[pcounter][counter + 80] = packet_block[pcounter][counter + 80] ^ packet_block[pcounter][counter + 96];
                                                packet_block[pcounter][counter + 64] = packet_block[pcounter][counter + 64] ^ packet_block[pcounter][counter + 80];
                                                packet_block[pcounter][counter + 48] = packet_block[pcounter][counter + 48] ^ packet_block[pcounter][counter + 64];
                                                packet_block[pcounter][counter + 32] = packet_block[pcounter][counter + 32] ^ packet_block[pcounter][counter + 48];
                                                packet_block[pcounter][counter + 16] = packet_block[pcounter][counter + 16] ^ packet_block[pcounter][counter + 32];
                                                packet_block[pcounter][counter] = packet_block[pcounter][counter] ^ packet_block[pcounter][counter + 16];
                                        }
                                        tmp_byte = packet_block[pcounter][255];
                                        for(uint16_t scounter = 255; scounter > 0; scounter --){
                                                packet_block[pcounter][scounter] = packet_block[pcounter][scounter - 1];
                                        }
                                        packet_block[pcounter][0] = tmp_byte;
                                        /*swapp*/
                                }
                        }
                        /*transpotitioning*/
                        /*substitution*/
                        rotor_number = 0;
                        search_byte = 0;
                        file_byte = 0;
                        rotation_counter_reset = 0;
                        swap_section_counter = 0;
                        ciphered_txt_byte = rotation_block[rotation_b_size - 1];
                        for(uint64_t round_counter = 0; round_counter < packets; round_counter ++){
                                for(uint16_t pcounter = 0; pcounter < 256; pcounter ++){
					/*rotateSwapKey.c*/
					rotateSwapKey(cells8, cells16, cells64, key_block, swap_block, rotation_block);
					/*rotateSwapKey.c*/
                                        ciphered_txt_byte = packet_block[round_counter][pcounter];
                                        for(uint64_t counter  = key_b_size; counter > 0; counter --){
                                                search_byte = packet_block[round_counter][pcounter];
                                                rotor_number = key_block[counter - 1];
                                                packet_block[round_counter][pcounter] = rev_rotor[rotor_number][search_byte];
                                        }
                                }
                                for(uint16_t xcounter = 255; xcounter > 0; xcounter --){
                                        packet_block[round_counter][xcounter] = packet_block[round_counter][xcounter] ^ packet_block[round_counter][xcounter - 1];
                                }
                        }
                        /*substitution*/
                free(temp_transpo);
                }
                if(single_rotation == 1){
                        /*transpotitioning*/
                        uint8_t *temp_transpo;
                        temp_transpo = (uint8_t*)malloc(256 * sizeof(uint8_t));
                        swap_section_counter = 0;
                        rotation_counter_reset = 0;
                        ciphered_txt_byte = rotation_block[rotation_b_size - 2];
                        for(uint64_t pcounter = 0; pcounter < packets; pcounter ++){
				/*rotateTranspoKey.c*/
				rotateTranspoKey(cells8, cells16, cells64, transpotition_block, transpo_swap_block, rotation_block);
				/*rotateTranspoKey.c*/
                                ciphered_txt_byte = packet_block[pcounter][ packet_block[pcounter][0] ];
                                for(uint64_t tcounter = transpotition_b_size; tcounter > 0; tcounter --){
                                        for(uint16_t counter  = 0; counter < 256; counter ++){
                                                temp_transpo[counter] = packet_block[pcounter][counter];
                                        }
                                        transpo_select = transpotition_block[tcounter - 1];
                                        for(uint16_t counter  = 0; counter < 256; counter ++){
                                                packet_block[pcounter][counter] = temp_transpo[rev_transpo[transpo_select][counter]];
                                        }
                                        /*swapp*/
                                        for(uint8_t counter = 0; counter < 16; counter ++){
                                                packet_block[pcounter][counter + 240] = packet_block[pcounter][counter + 240] ^ packet_block[pcounter][counter];
                                                packet_block[pcounter][counter + 224] = packet_block[pcounter][counter + 224] ^ packet_block[pcounter][counter + 240];
                                                packet_block[pcounter][counter + 208] = packet_block[pcounter][counter + 208] ^ packet_block[pcounter][counter + 224];
                                                packet_block[pcounter][counter + 192] = packet_block[pcounter][counter + 192] ^ packet_block[pcounter][counter + 208];
                                                packet_block[pcounter][counter + 176] = packet_block[pcounter][counter + 176] ^ packet_block[pcounter][counter + 192];
                                                packet_block[pcounter][counter + 160] = packet_block[pcounter][counter + 160] ^ packet_block[pcounter][counter + 176];
                                                packet_block[pcounter][counter + 144] = packet_block[pcounter][counter + 144] ^ packet_block[pcounter][counter + 160];
                                                packet_block[pcounter][counter + 128] = packet_block[pcounter][counter + 128] ^ packet_block[pcounter][counter + 144];
                                                packet_block[pcounter][counter + 112] = packet_block[pcounter][counter + 112] ^ packet_block[pcounter][counter + 128];
                                                packet_block[pcounter][counter + 96] = packet_block[pcounter][counter + 96] ^ packet_block[pcounter][counter + 112];
                                                packet_block[pcounter][counter + 80] = packet_block[pcounter][counter + 80] ^ packet_block[pcounter][counter + 96];
                                                packet_block[pcounter][counter + 64] = packet_block[pcounter][counter + 64] ^ packet_block[pcounter][counter + 80];
                                                packet_block[pcounter][counter + 48] = packet_block[pcounter][counter + 48] ^ packet_block[pcounter][counter + 64];
                                                packet_block[pcounter][counter + 32] = packet_block[pcounter][counter + 32] ^ packet_block[pcounter][counter + 48];
                                                packet_block[pcounter][counter + 16] = packet_block[pcounter][counter + 16] ^ packet_block[pcounter][counter + 32];
                                                packet_block[pcounter][counter] = packet_block[pcounter][counter] ^ packet_block[pcounter][counter + 16];
                                        }
                                        tmp_byte = packet_block[pcounter][255];
                                        for(uint16_t scounter = 255; scounter > 0; scounter --){
                                                packet_block[pcounter][scounter] = packet_block[pcounter][scounter - 1];
                                        }
                                        packet_block[pcounter][0] = tmp_byte;
                                        /*swapp*/
                                }
                        }
                        /*transpotitioning*/
                        /*substitution*/
                        rotor_number = 0;
                        search_byte = 0;
                        file_byte = 0;
                        rotation_counter_reset = 0;
                        swap_section_counter = 0;
                        ciphered_txt_byte = rotation_block[rotation_b_size - 1];
                        for(uint64_t pcounter = 0; pcounter < packets; pcounter ++){
				/*rotateSwapKey.c*/
				rotateSwapKey(cells8, cells16, cells64, key_block, swap_block, rotation_block);
				/*rotateSwapKey.c*/
                                ciphered_txt_byte = packet_block[pcounter][ packet_block[pcounter][0] ];
                                for(uint64_t tcounter = key_b_size; tcounter > 0; tcounter --){
                                        rotor_number = key_block[tcounter - 1];
                                        for(uint16_t ctr = 0; ctr < 256; ctr ++){
                                                search_byte = packet_block[pcounter][ctr];
                                                packet_block[pcounter][ctr] = rev_rotor[rotor_number][search_byte];
                                        }
                                        /*swapp*/
                                        for(uint8_t counter = 0; counter < 16; counter ++){
                                                packet_block[pcounter][counter + 240] = packet_block[pcounter][counter + 240] ^ packet_block[pcounter][counter];
                                                packet_block[pcounter][counter + 224] = packet_block[pcounter][counter + 224] ^ packet_block[pcounter][counter + 240];
                                                packet_block[pcounter][counter + 208] = packet_block[pcounter][counter + 208] ^ packet_block[pcounter][counter + 224];
                                                packet_block[pcounter][counter + 192] = packet_block[pcounter][counter + 192] ^ packet_block[pcounter][counter + 208];
                                                packet_block[pcounter][counter + 176] = packet_block[pcounter][counter + 176] ^ packet_block[pcounter][counter + 192];
                                                packet_block[pcounter][counter + 160] = packet_block[pcounter][counter + 160] ^ packet_block[pcounter][counter + 176];
                                                packet_block[pcounter][counter + 144] = packet_block[pcounter][counter + 144] ^ packet_block[pcounter][counter + 160];
                                                packet_block[pcounter][counter + 128] = packet_block[pcounter][counter + 128] ^ packet_block[pcounter][counter + 144];
                                                packet_block[pcounter][counter + 112] = packet_block[pcounter][counter + 112] ^ packet_block[pcounter][counter + 128];
                                                packet_block[pcounter][counter + 96] = packet_block[pcounter][counter + 96] ^ packet_block[pcounter][counter + 112];
                                                packet_block[pcounter][counter + 80] = packet_block[pcounter][counter + 80] ^ packet_block[pcounter][counter + 96];
                                                packet_block[pcounter][counter + 64] = packet_block[pcounter][counter + 64] ^ packet_block[pcounter][counter + 80];
                                                packet_block[pcounter][counter + 48] = packet_block[pcounter][counter + 48] ^ packet_block[pcounter][counter + 64];
                                                packet_block[pcounter][counter + 32] = packet_block[pcounter][counter + 32] ^ packet_block[pcounter][counter + 48];
                                                packet_block[pcounter][counter + 16] = packet_block[pcounter][counter + 16] ^ packet_block[pcounter][counter + 32];
                                                packet_block[pcounter][counter] = packet_block[pcounter][counter] ^ packet_block[pcounter][counter + 16];
                                        }
                                        for(uint16_t scounter = 255; scounter > 0; scounter --){
                                                packet_block[pcounter][scounter] = packet_block[pcounter][scounter] ^ packet_block[pcounter][scounter - 1];
                                        }
                                        /*swapp*/
                                }
                        }
                        /*substitution*/
                free(temp_transpo);
                }
                if(multimixed_rotation == 1){
                        //temp var saving
                        uint64_t trotation = 0, tswap = 0;
                        uint64_t srotation = 0, sswap = 0;
                        uint8_t tciphered, sciphered;
                        //temp var saving
                        /*transpotitioning*/
                        uint8_t *temp_transpo;
                        temp_transpo = (uint8_t*)malloc(256 * sizeof(uint8_t));
                        swap_section_counter = 0;
                        rotation_counter_reset = 0;
                        tciphered = rotation_block[rotation_b_size - 2];
                        /*transpotitioning*/
                        /*substitution*/
                        rotor_number = 0;
                        search_byte = 0;
                        file_byte = 0;
                        rotation_counter_reset = 0;
                        swap_section_counter = 0;
                        sciphered = rotation_block[rotation_b_size - 1];
                        /*substitution*/
                        /*sub keys*/
                        uint8_t **sub_keys;
                        sub_keys = (uint8_t**)malloc(multimixed_turns * sizeof(uint8_t*));
                        uint8_t **sub_tables;
                        sub_tables = (uint8_t**)malloc(multimixed_turns * sizeof(uint8_t*));
                        for(uint16_t k = 0; k < multimixed_turns; k ++){
                                sub_keys[k] = (uint8_t*)malloc(key_b_size * sizeof(uint8_t));
                                sub_tables[k] = (uint8_t*)malloc(transpotition_b_size * sizeof(uint8_t));
                        }
                        /*sub keys*/
                        for(uint64_t pcounter = 0; pcounter < packets; pcounter ++){
                                /*subkeys generate*/
                                for(uint16_t kturns = multimixed_turns; kturns > 0; kturns --){
                                        ciphered_txt_byte = tciphered;
                                        rotation_counter_reset = trotation;
                                        swap_section_counter = tswap;

					/*rotateTranspoKey.c*/
					rotateTranspoKey(cells8, cells16, cells64, transpotition_block, transpo_swap_block, rotation_block);
					/*rotateTranspoKey.c*/
                                        trotation = rotation_counter_reset;
                                        tswap = swap_section_counter;
                                        for(uint64_t te = 0; te < transpotition_b_size; te ++){
                                                sub_tables[kturns - 1][te] = transpotition_block[te];
                                        }

                                        ciphered_txt_byte = sciphered;
                                        rotation_counter_reset = srotation;
                                        swap_section_counter = sswap;

					/*rotateSwapKey.c*/
					rotateSwapKey(cells8, cells16, cells64, key_block, swap_block, rotation_block);
					/*rotateSwapKey.c*/
                                        srotation = rotation_counter_reset;
                                        sswap = swap_section_counter;
                                        for(uint64_t ke = 0; ke < key_b_size; ke ++){
                                                sub_keys[kturns - 1][ke] = key_block[ke];
                                        }
                                }
                                /*subkeys generate*/

                                tciphered = packet_block[pcounter][ packet_block[pcounter][255] ];
                                sciphered = packet_block[pcounter][ packet_block[pcounter][0] ];

                                for(uint16_t turns = 0; turns < multimixed_turns; turns ++){
                                        /*transpotitioning*/
                                        for(uint64_t tcounter = transpotition_b_size; tcounter > 0; tcounter --){
                                                for(uint16_t counter  = 0; counter < 256; counter ++){
                                                        temp_transpo[counter] = packet_block[pcounter][counter];
                                                }
                                                transpo_select = sub_tables[turns][tcounter - 1];
                                                for(uint16_t counter  = 0; counter < 256; counter ++){
                                                        packet_block[pcounter][counter] = temp_transpo[rev_transpo[transpo_select][counter]];
                                                }
                                                /*swapp*/
                                                for(uint8_t counter = 0; counter < 16; counter ++){
                                                        packet_block[pcounter][counter + 240] = packet_block[pcounter][counter + 240] ^ packet_block[pcounter][counter];
                                                        packet_block[pcounter][counter + 224] = packet_block[pcounter][counter + 224] ^ packet_block[pcounter][counter + 240];
                                                        packet_block[pcounter][counter + 208] = packet_block[pcounter][counter + 208] ^ packet_block[pcounter][counter + 224];
                                                        packet_block[pcounter][counter + 192] = packet_block[pcounter][counter + 192] ^ packet_block[pcounter][counter + 208];
                                                        packet_block[pcounter][counter + 176] = packet_block[pcounter][counter + 176] ^ packet_block[pcounter][counter + 192];
                                                        packet_block[pcounter][counter + 160] = packet_block[pcounter][counter + 160] ^ packet_block[pcounter][counter + 176];
                                                        packet_block[pcounter][counter + 144] = packet_block[pcounter][counter + 144] ^ packet_block[pcounter][counter + 160];
                                                        packet_block[pcounter][counter + 128] = packet_block[pcounter][counter + 128] ^ packet_block[pcounter][counter + 144];
                                                        packet_block[pcounter][counter + 112] = packet_block[pcounter][counter + 112] ^ packet_block[pcounter][counter + 128];
                                                        packet_block[pcounter][counter + 96] = packet_block[pcounter][counter + 96] ^ packet_block[pcounter][counter + 112];
                                                        packet_block[pcounter][counter + 80] = packet_block[pcounter][counter + 80] ^ packet_block[pcounter][counter + 96];
                                                        packet_block[pcounter][counter + 64] = packet_block[pcounter][counter + 64] ^ packet_block[pcounter][counter + 80];
                                                        packet_block[pcounter][counter + 48] = packet_block[pcounter][counter + 48] ^ packet_block[pcounter][counter + 64];
                                                        packet_block[pcounter][counter + 32] = packet_block[pcounter][counter + 32] ^ packet_block[pcounter][counter + 48];
                                                        packet_block[pcounter][counter + 16] = packet_block[pcounter][counter + 16] ^ packet_block[pcounter][counter + 32];
                                                        packet_block[pcounter][counter] = packet_block[pcounter][counter] ^ packet_block[pcounter][counter + 16];
                                                }
                                                tmp_byte = packet_block[pcounter][255];
                                                for(uint16_t scounter = 255; scounter > 0; scounter --){
                                                        packet_block[pcounter][scounter] = packet_block[pcounter][scounter - 1];
                                                }
                                                packet_block[pcounter][0] = tmp_byte;
                                                /*swapp*/
                                        /*transpotitioning*/

                                        /*substitution*/
                                                rotor_number = sub_keys[turns][tcounter - 1];
                                                for(uint16_t ctr = 0; ctr < 256; ctr ++){
                                                        search_byte = packet_block[pcounter][ctr];
                                                        packet_block[pcounter][ctr] = rev_rotor[rotor_number][search_byte];
                                                }
                                                /*swapp*/
                                                for(uint8_t counter = 0; counter < 16; counter ++){
                                                        packet_block[pcounter][counter + 240] = packet_block[pcounter][counter + 240] ^ packet_block[pcounter][counter];
                                                        packet_block[pcounter][counter + 224] = packet_block[pcounter][counter + 224] ^ packet_block[pcounter][counter + 240];
                                                        packet_block[pcounter][counter + 208] = packet_block[pcounter][counter + 208] ^ packet_block[pcounter][counter + 224];
                                                        packet_block[pcounter][counter + 192] = packet_block[pcounter][counter + 192] ^ packet_block[pcounter][counter + 208];
                                                        packet_block[pcounter][counter + 176] = packet_block[pcounter][counter + 176] ^ packet_block[pcounter][counter + 192];
                                                        packet_block[pcounter][counter + 160] = packet_block[pcounter][counter + 160] ^ packet_block[pcounter][counter + 176];
                                                        packet_block[pcounter][counter + 144] = packet_block[pcounter][counter + 144] ^ packet_block[pcounter][counter + 160];
                                                        packet_block[pcounter][counter + 128] = packet_block[pcounter][counter + 128] ^ packet_block[pcounter][counter + 144];
                                                        packet_block[pcounter][counter + 112] = packet_block[pcounter][counter + 112] ^ packet_block[pcounter][counter + 128];
                                                        packet_block[pcounter][counter + 96] = packet_block[pcounter][counter + 96] ^ packet_block[pcounter][counter + 112];
                                                        packet_block[pcounter][counter + 80] = packet_block[pcounter][counter + 80] ^ packet_block[pcounter][counter + 96];
                                                        packet_block[pcounter][counter + 64] = packet_block[pcounter][counter + 64] ^ packet_block[pcounter][counter + 80];
                                                        packet_block[pcounter][counter + 48] = packet_block[pcounter][counter + 48] ^ packet_block[pcounter][counter + 64];
                                                        packet_block[pcounter][counter + 32] = packet_block[pcounter][counter + 32] ^ packet_block[pcounter][counter + 48];
                                                        packet_block[pcounter][counter + 16] = packet_block[pcounter][counter + 16] ^ packet_block[pcounter][counter + 32];
                                                        packet_block[pcounter][counter] = packet_block[pcounter][counter] ^ packet_block[pcounter][counter + 16];
                                                }
                                                for(uint16_t scounter = 255; scounter > 0; scounter --){
                                                        packet_block[pcounter][scounter] = packet_block[pcounter][scounter] ^ packet_block[pcounter][scounter - 1];
                                                }
                                                /*swapp*/
                                        }
                                        /*substitution*/
                                }
                        }

                free(sub_keys);
                free(sub_tables);
                free(temp_transpo);
                }
                /******************************* ALGORITHM ****************************************/

                /*checksum check*/
                check_error = 0;
                for(uint64_t pcounter = 0; pcounter < packets; pcounter ++){
                        checksum = 0;
                        for(uint16_t counter = 16; counter < 256; counter ++){
                                checksum = checksum + packet_block[pcounter][counter];
                        }
                        checksum_multi = 0;
                        for(uint16_t counter = 16; counter < 255;  counter ++){
                                checksum_multi = checksum_multi + (packet_block[pcounter][counter] * packet_block[pcounter][counter + 1]);
                        }
                        check_less = 0;
                        reverse_packet_counter = 255;
                        for(uint16_t counter = 16; counter < 136;  counter ++){
                                check_less = check_less + (packet_block[pcounter][counter] * packet_block[pcounter][reverse_packet_counter]);
                                reverse_packet_counter --;
                        }
                        checksum_multi_rev = 64000;
                        for(uint16_t counter = 16; counter < 256; counter = counter + 2){
                                checksum_multi_rev = checksum_multi_rev + packet_block[pcounter][counter];
                                checksum_multi_rev = checksum_multi_rev - packet_block[pcounter][counter + 1];
                        }
                        check_cmp = checksum * checksum_multi * check_less * checksum_multi_rev;
                        //extracts sum from file
                        checksum_total = 0;
                        for(uint8_t counter = 0; counter < 8; counter ++){
                                checksum_total = checksum_total << 8;
                                checksum_total = checksum_total & 0xffffffffffffff00;
                                checksum_total = checksum_total | packet_block[pcounter][counter + 8];
                        }
                        if(checksum_total != check_cmp){
                                check_error ++ ;
                        }
                }
                /*checksum check*/

                /*extract lengh info from packets*/
                packet_errors = 0;
                output_file_size = 0;
                for(uint64_t counter  = 0; counter < packets; counter ++){
                        if(counter != packets - 1){
                                if(packet_block[counter][3] != 240){
                                        packet_errors ++;
                                        if(packet_block[counter][3] > 240){
						packet_block[counter][3] = 240;
                                        }
                                }
                        }
                        if(counter == packets - 1){
                                if(packet_block[counter][3] > 240){
					packet_block[counter][3] = 240;
                                }
                        }
                        output_file_size = output_file_size + packet_block[counter][3];
                }
                /*extract original file from packets*/

                /*output file*/
                uint8_t *output_block;
                output_block = (uint8_t*)malloc(output_file_size * sizeof(uint8_t));
                file_byte = 0;
                for(uint64_t pcounter  = 0; pcounter < packets; pcounter ++){
                        for(uint16_t counter = 0; counter < packet_block[pcounter][3]; counter ++){
                                output_block[file_byte] = packet_block[pcounter][counter + 16];
                                file_byte ++;
                        }
                }
                /*output file*/

                /*resets key*/
                for(uint64_t counter = 0; counter < key_b_size; counter ++){
                        key_block[counter] = first_key_block[counter];
                }
                swap_section_counter = 0;
                /*resets key*/

                /*xor file with key*/
                key_counter_reset = key_b_size;
                rotation_counter_reset = 0;
                swap_section_counter = 0;
                for(uint64_t counter = 0; counter < output_file_size; counter ++){
                        if(key_counter_reset >= key_b_size){
                                key_counter_reset = 0;
				/*rotateFirstKey.c*/
				rotateFirstKey(cells8, cells16, cells64, key_block, fswap_block, rotation_block);
				/*rotateFirstKey.c*/
                        }
                        output_block[counter] = output_block[counter] ^ key_block[key_counter_reset];
                        key_counter_reset ++;
                }
                /*xor file with key*/
                /*print on  screen*/
                if(print_option == 1){
                        if(print_hex == 0 && print_upp_hex == 0){
                                for(uint64_t counter = 0; counter < output_file_size; counter ++){
                                        printf("%c", output_block[counter]);
                                }
                                if(output_block[output_file_size - 1] != 0x0a){
                                        printf("\n");
                                }
                        }
                        if(print_hex == 1){
                                rotation_counter_reset = 0;
                                for(uint64_t counter = 0; counter < output_file_size; counter ++){
                                        if(rotation_counter_reset == (column_number / 2)){
                                                ////////////
                                                printf("   ");
                                                for(uint32_t pasc = 0; pasc < (column_number / 2); pasc ++){
                                                        if(output_block[counter - rotation_counter_reset + pasc] < 0x20){
                                                                printf(".");
                                                        }
                                                        if(output_block[counter - rotation_counter_reset + pasc] > 0x7e){
                                                                printf(".");
                                                        }
                                                        if(output_block[counter - rotation_counter_reset + pasc] > 0x1f &&  output_block[counter - rotation_counter_reset + pasc] < 0x7f){
                                                                printf("%c", output_block[counter - rotation_counter_reset + pasc]);
                                                        }
                                                }
                                                ////////////
                                                rotation_counter_reset = 0;
                                                printf("\n");
                                        }
                                        printf("%02"PRIx8, output_block[counter]);
                                        rotation_counter_reset ++;
                                }
                                ////////////
                                if(rotation_counter_reset <= (column_number / 2)){
                                        printf("   ");
                                        for(uint32_t pasc = 0; pasc < (column_number / 2) -  rotation_counter_reset; pasc ++){
                                                printf("  ");
                                        }
                                        for(uint32_t pasc = rotation_counter_reset; pasc > 0; pasc --){
                                                if(output_block[output_file_size - pasc] < 0x20){
                                                        printf(".");
                                                }
                                                if(output_block[output_file_size - pasc] > 0x7e){
                                                        printf(".");
                                                }
                                                if(output_block[output_file_size - pasc] > 0x1f &&  output_block[output_file_size - pasc] < 0x7f){
                                                        printf("%c", output_block[output_file_size - pasc]);
                                                }
                                        }
                                }
                                ////////////
                                printf("\n");
                        }
                        if(print_upp_hex == 1){
                                rotation_counter_reset = 0;
                                for(uint64_t counter = 0; counter < output_file_size; counter ++){
                                        if(rotation_counter_reset == (column_number / 2)){
                                                /////print ascii///////
                                                printf("   ");
                                                for(uint32_t pasc = 0; pasc < (column_number / 2); pasc ++){
                                                        if(output_block[counter - rotation_counter_reset + pasc] < 0x20){
                                                                printf(".");
                                                        }
                                                        if(output_block[counter - rotation_counter_reset + pasc] > 0x7e){
                                                                printf(".");
                                                        }
                                                        if(output_block[counter - rotation_counter_reset + pasc] > 0x1f &&  output_block[counter - rotation_counter_reset + pasc] < 0x7f){
                                                                printf("%c", output_block[counter - rotation_counter_reset + pasc]);
                                                        }
                                                }
                                                /////print ascii///////
                                                rotation_counter_reset = 0;
                                                printf("\n");
                                        }
                                        printf("%02"PRIX8, output_block[counter]);
                                        rotation_counter_reset ++;
                                }
                                //////print ascii//////
                                if(rotation_counter_reset <= (column_number / 2)){
                                        printf("   ");
                                        for(uint32_t pasc = 0; pasc < (column_number / 2) -  rotation_counter_reset; pasc ++){
                                                printf("  ");
                                        }
                                        for(uint32_t pasc = rotation_counter_reset; pasc > 0; pasc --){
                                                if(output_block[output_file_size - pasc] < 0x20){
                                                        printf(".");
                                                }
                                                if(output_block[output_file_size - pasc] > 0x7e){
                                                        printf(".");
                                                }
                                                if(output_block[output_file_size - pasc] > 0x1f &&  output_block[output_file_size - pasc] < 0x7f){
                                                        printf("%c", output_block[output_file_size - pasc]);
                                                }
                                        }
                                }
                                //////print ascii//////
                                printf("\n");
                        }
                }
                /*print on  screen*/
                /*saving to out file*/
                if(print_option == 0){
                        FILE *output_file;
                        output_file = fopen(output_file_name, output_mode);
                        if(output_file == NULL){
                                printf("couldn't open %s.\n", output_file_name);
                                free(output_block);
                                free(input_block);
                                free(packet_block);
                                exit_code = 10;
                                goto __emergency_exit;
                        }
                        /*+++++++ckeck output size++++++++++++*/
                        if(output_required == 1){
                                fseek(output_file, 0, SEEK_END);
                                drive_size = ftell(output_file);
                                if(drive_size - 2 < output_address + output_file_size){
                                         printf("output boundary error. max start address is %"PRIu64".  %s size is %"PRIu64" bytes.\n", drive_size - output_file_size, output_file_name ,drive_size);
                                        fclose(output_file);
                                        free(output_block);
                                        free(input_block);
                                        free(packet_block);
                                        exit_code = 10;
                                        goto __emergency_exit;
                                }
                        }
                        /*+++++++ckeck output size++++++++++++*/
                        fseek(output_file, output_address, SEEK_SET);
                        fwrite(output_block, output_file_size, 1, output_file);
                        fclose(output_file);
                }
                /*saving to out file*/

                /*results if output required*/
                if(output_required == 1){
                        printf("output %"PRIu64" bytes.  addresses: %"PRIu64" - %"PRIu64"\n", output_file_size, output_address, output_address + (output_file_size - 1));
                }
                /*results if output required*/

                free(packet_block);
                free(output_block);
                free(input_block);
                exit_code = 0;
        }
///////////////////////////////////////////////////DECIPHERING/////////////////////////////////////////////////////////////////////////////////

__emergency_exit:
        /*closing files*/
        free(lock_block);
        free(key_block);
        free(first_key_block);
        free(rotor);
        free(rev_rotor);
        free(transpo);
        free(rev_transpo);
        free(transpotition_block);
        free(rotation_block);
        free(swap_block);
        free(fswap_block);
        free(transpo_swap_block);
        /*closing files*/
        gettimeofday(&stop, NULL);

        if(print_option == 0){
                printf("total time %ld secs.\n", stop.tv_sec - start.tv_sec);
        }

        if(check_error > 0 || packet_errors > 0){
                printf("packet integrity errors found.\n");
        }
        if(verify_rt == 0){
                if(verify_errors > 0 || duplicate_errors > 0){
                        printf("cipher-lock-256bit integrity errors found.\n");
                }
        }
	return 0;
}
//////////////////////////////////////////////////END/////////////////////////////////////////////
