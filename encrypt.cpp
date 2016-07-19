#include "rice/Data_Type.hpp"
#include "rice/Constructor.hpp"
#include <iostream>

#include <iomanip>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/rdrand.h>
#include <cryptopp/osrng.h>
#include <cryptopp/des.h>
#include <cryptopp/hex.h>

using namespace CryptoPP;
using namespace std;

using namespace Rice;

class Encrypt
{
public:
  virtual ~Encrypt() = 0;
  virtual char const * encryptdecrypt() = 0;
};

Encrypt::
~Encrypt()
{
}


class Aesencryption
  : public Encrypt
{
public:
  virtual char const * encryptdecrypt()
  {

        AutoSeededRandomPool rnd;
	  	char recovered []= "";
	  	char cipher []= "";

	  	SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
	  	rnd.GenerateBlock( key, key.size() );

	  	byte iv[AES::BLOCKSIZE];
	  	rnd.GenerateBlock(iv, AES::BLOCKSIZE);

	  	char plainText[] = "Hello! How are you?";
	  	int messageLen = (int)strlen(plainText) + 1;

	  	CFB_Mode<AES>::Encryption cfbEncryption(key, key.size(), iv);
	  	cfbEncryption.ProcessData((byte*)cipher, (byte*)plainText, messageLen);

	  	cout << "cipher text: " << cipher << endl;

	  	CFB_Mode<AES>::Decryption cfbDecryption(key, key.size(), iv);
	  	cfbDecryption.ProcessData((byte*)recovered, (byte*)cipher, messageLen);

	  	cout << "plain text: " << plainText << endl;

    return "This is Aesencryption";
  }

};

class Desencryption
  : public Encrypt
{
public:
  virtual char const * encryptdecrypt()
  {
	   AutoSeededRandomPool rnd;
	  	char recovered []= "";
	  	char cipher []= "";

	  	SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
	  	rnd.GenerateBlock( key, key.size() );

	  	byte iv[AES::BLOCKSIZE];
	  	rnd.GenerateBlock(iv, AES::BLOCKSIZE);

	  	char plainText[] = "Hello! How are you today? ";
	  	int messageLen = (int)strlen(plainText) + 1;

	  	CFB_Mode<AES>::Encryption cfbEncryption(key, key.size(), iv);
	  	cfbEncryption.ProcessData((byte*)cipher, (byte*)plainText, messageLen);

	  	cout << "cipher text: " << cipher << endl;

	  	CFB_Mode<AES>::Decryption cfbDecryption(key, key.size(), iv);
	  	cfbDecryption.ProcessData((byte*)recovered, (byte*)cipher, messageLen);

	  	cout << "plain text:" << plainText << endl;
    return "This is Desencryption" ;
  }

};


extern "C"
void Init_encrypt(void)
{
  RUBY_TRY
  {
    define_class<Encrypt>("encrypt")
      .define_method("encryptdecrypt", &Encrypt::encryptdecrypt);

    define_class<Aesencryption, Encrypt>("Aesencryption")
      .define_constructor(Constructor<Aesencryption>());

    define_class<Desencryption, Encrypt>("Desencryption")
      .define_constructor(Constructor<Desencryption>());
  }
  RUBY_CATCH
}
