#include "DES.h"
#define FIRSTBIT 0x8000000000000000


DES::DES()
{
}


DES::~DES()
{
}


void DES::addbit(uint64_t *block, uint64_t from, int position_from, int position_to)
{
	if (((from << (position_from)) & FIRSTBIT) != 0) { *block += (FIRSTBIT >> position_to); }
}
void DES::Permutation(uint64_t* data, bool initial)
{
	uint64_t data_temp = 0;
	for (int ii = 0; ii < 64; ii++)
	{
		if (initial) { addbit(&data_temp, *data, InitialPermutation[ii] - 1, ii); }
		else { addbit(&data_temp, *data, FinalPermutation[ii] - 1, ii); }
	}
	*data = data_temp;
}
bool DES::key_parity_verify(uint64_t key)
{
	int parity_bit = 0; // Parity helper

	for (int ii = 0; ii < 64; ii++)
	{
		// Test the parity bit (8-th bit)
		if (ii % 8 == 7)
		{
			if (parity_bit == 0)
			{
				// Test if 8-th bit != 0
				if (((key << ii) & FIRSTBIT) != (uint64_t)0)
				{
					printf("parity error at bit #%i\n", ii + 1);
					return false;
				}
			}
			else
			{
				// Test if 8-th bit != 1
				if (((key << ii) & FIRSTBIT) != FIRSTBIT)
				{
					printf("parity error at bit #%i\n", ii + 1);
					return false;
				}

			}
			parity_bit = 0; // Re-init parity_bit for next byte block
		}
		// Count numbers of 1 in the 7bit block
		else
		{
			if (((key << ii) & FIRSTBIT) == FIRSTBIT) { parity_bit = parity_bit == 0 ? 1 : 0; }
		}
	}
	return true;
}
void DES::key_schedule(uint64_t* key, uint64_t* next_key, int round)
{
	// Init
	uint64_t key_left = 0;
	uint64_t key_right = 0;

	uint64_t key_left_temp = 0;
	uint64_t key_right_temp = 0;

	*next_key = 0; // Important !

				   // 1. First round => PC-1 : Permuted Choice 1
	if (round == 0)
	{
		for (int ii = 0; ii < 56; ii++)
		{
			if (ii < 28)
				addbit(&key_left, *key, PC1[ii] - 1, ii);
			else
				addbit(&key_right, *key, PC1[ii] - 1, ii % 28);
		}
	}
	// 1'. Other rounds? => Seperate key into two key halves.
	else
	{
		for (int ii = 0; ii < 56; ii++)
		{
			if (ii < 28)
				addbit(&key_left, *key, ii, ii);
			else
				addbit(&key_right, *key, ii, ii % 28);
		}
	}

	// 2. Rotations
	key_left_temp = Rotations[round] == 1 ? FIRSTBIT : 0xC000000000000000;
	key_right_temp = Rotations[round] == 1 ? FIRSTBIT : 0xC000000000000000;
	key_left_temp = (key_left & key_left_temp) >> (28 - Rotations[round]);
	key_right_temp = (key_right & key_right_temp) >> (28 - Rotations[round]);

	key_left_temp += (key_left << Rotations[round]);
	key_right_temp += (key_right << Rotations[round]);

	// Combine the 2 keys into 1 (next_key)
	// Next_key will be used for following rounds
	for (int ii = 0; ii < 56; ii++)
	{
		if (ii < 28)
			addbit(next_key, key_left_temp, ii, ii);
		else
			addbit(next_key, key_right_temp, (ii % 28), ii);
	}

	// 3. PC-2 : Permuted Choice 2
	*key = 0;

	for (int ii = 0; ii < 48; ii++)
		addbit(key, *next_key, PC2[ii] - 1, ii);
}
void DES::rounds(uint64_t *data, uint64_t key)
{
	uint64_t right_block = 0;
	uint64_t right_block_temp = 0;

	// 1. Block expansion
	for (int ii = 0; ii < 48; ii++)
		addbit(&right_block, *data, (DesExpansion[ii] + 31), ii);

	// 2. Xor with the key
	right_block = right_block ^ key;

	// 3. Substitution
	int coordx, coordy;
	uint64_t substitued;

	for (int ii = 0; ii < 8; ii++)
	{
		coordx = ((right_block << 6 * ii) & FIRSTBIT) == FIRSTBIT ? 2 : 0;
		if (((right_block << (6 * ii + 5)) & FIRSTBIT) == FIRSTBIT)
			coordx++;

		coordy = 0;
		for (int jj = 1; jj < 5; jj++)
		{
			if (((right_block << (6 * ii + jj)) & FIRSTBIT) == FIRSTBIT)
			{
				coordy += 2 ^ (4 - jj);
			}
		}

		substitued = DesSbox[ii][coordx][coordy];
		substitued = substitued << (60 - (4 * ii));
		right_block_temp += substitued;
	}

	// Right_block done
	right_block = right_block_temp;

	// 4. Permutation
	right_block_temp = 0;

	for (int ii = 0; ii < 32; ii++)
		addbit(&right_block_temp, right_block, Pbox[ii] - 1, ii);

	right_block = right_block_temp;

	// 5. Xor with the left block
	right_block = right_block ^ *data;

	// Combine the new block and the right block
	*data = (*data << 32) + (right_block >> 32);
}
std::string DES::stringToBinary(std::string yourString)
{
	std::string binary_outputInformations;
	for (std::size_t i = 0; i < yourString.size(); ++i)
	{
		std::bitset<8> bs(yourString.c_str()[i]);
		binary_outputInformations += bs.to_string();
	}

	return binary_outputInformations;
}
uint64_t DES::binStringToUint(std::string binary)
{
	uint64_t uint = 0;
	uint64_t exponent = 0;
	std::string::reverse_iterator iter;

	for (iter = binary.rbegin(); iter != binary.rend(); iter++)
	{
		if (*iter == '1')
			uint += (FIRSTBIT >> exponent);

		exponent++;
	}

	return uint;
}
 std::string DES::binaryToString(std::string binary)
 {
	std::stringstream sstream(binary);
	std::string output;
	 while (sstream.good())
	 {
		std::bitset<8> bits;
		sstream >> bits;
		char c = char(bits.to_ulong());
		output += c;
	}

	return output;
 }
 std::string DES::UintToBinary(uint64_t decimal)
 {
	std::string binary, newBinary = "";
	std::string::reverse_iterator iter;
	uint64_t remainder;

	while (decimal > 0)
	{
		remainder = decimal % 2;

	if (remainder == 0)
	binary += '0';
	else if (remainder == 1)
	binary += '1';

	decimal /= 2;
	}

	for (iter = binary.rbegin(); iter != binary.rend(); iter++)
	{
		newBinary += *iter;
	}

	return newBinary;
 }

void DES::DESAlgorithm(bool encDecOption, const char* input, const char* output)
{
	std::string secretKey = "";
	std::cout << "Podaj klucz" << std::endl;
	Utilities::maskPassword(secretKey);
	//wczytaj klucz od uzytkownika

	DES *des = new DES();

	FILE * in = NULL;
	FILE * out = NULL;
	fopen_s(&in, input, "rb");
	fopen_s(&out, output, "wb");

	//Convert Key to binary and then to decimal
	std::string secretKeyBin = des->stringToBinary(secretKey);
	uint64_t secretKeyDec = des->binStringToUint(secretKeyBin);

	uint64_t a_key[16];
	a_key[0] = secretKeyDec;
	uint64_t next_key;

	//Get the 16 subkeys
	for (int ii = 0; ii < 16; ii++) //dla 0 problem
	{
		des->key_schedule(&a_key[ii], &next_key, ii);
		if (ii != 15)
			a_key[ii + 1] = next_key;
	}

	//16 Rounds of enc/decryption
	size_t amount; // Used for fwrite
	uint64_t data;

	while ((amount = fread(&data, 1, 8, in)) > 0)
	{
		if (amount != 8)
			data = data << (8 * (8 - amount));

		// Initial permutation
		des->Permutation(&data, true);

		// Encrypt rounds
		if (encDecOption == 0)
		{
			for (int ii = 0; ii < 16; ii++)
				des->rounds(&data, a_key[ii]);
		}
		// Decrypt rounds
		else
		{
			// Switching blocks
			data = (data << 32) + (data >> 32);

			for (int ii = 15; ii >= 0; ii--)
				des->rounds(&data, a_key[ii]);

			// Switching blocks back
			data = (data << 32) + (data >> 32);
		}

		// Final permutation
		des->Permutation(&data, false);

		if (amount != 8)
			data = data << (8 * (8 - amount));


		fwrite(&data, 1, amount, out);

		data = 0;
	}
	fclose(in);
	fclose(out);

}
void DES::EncryptDecrypt(bool EncDecOption, std::string SecretData) //if EncDecOption is true then Encryption else Decryption
{
	//type secret key
	if (EncDecOption == true)
	{
		// 1.Save data typed by user to .bin file
		if (SecretData.length() % 8 != 0) { SecretData.insert(SecretData.length(), 8 - SecretData.length() % 8, ' '); }
		SecretData += '\n';

		FILE *file = NULL;
		fopen_s(&file, "log.bin", "wb");
		fwrite(SecretData.c_str(), 1, SecretData.size(), file);
		fclose(file);

		// 2. Encrypt data and save them to config file
		DESAlgorithm(true, "log.bin", "config.bin");

		// 3. Delete log.bin file
		remove("log.bin");
	}
	else
	{
		DESAlgorithm(false, "config.bin", "log.bin");
	}
}

void DES::readEncryptedData(Camera *&camera)
{
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	fopen_s(&pFile, "log.bin", "rb");
	if (pFile == NULL) { fputs("File error", stderr); exit(1); }

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	// allocate memory to contain the whole file:
	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

	// copy the file into the buffer:
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize) { fputs("Reading error", stderr); exit(3); }

	std::string buf(buffer);
	int newLinePosition = buf.find(' ');
	buf = buf.substr(0, newLinePosition);
	newLinePosition = buf.find('\n');
	buf = buf.substr(0, newLinePosition);

	//sep[0] camera->getIPaddress()
	//sep[1] camera->getUSERPWD()
	std::vector<std::string> sep = Utilities::split(buf, ',');
	camera->setIPaddress(sep[0]);
	camera->setUSERPWD(sep[1]);

	// 3. Delete log.bin file
	remove("log.bin");
	fclose(pFile);
	free(buffer);
}
