#include "rice/Data_Type.hpp"
#include "rice/Constructor.hpp"
#include <iostream>

#include <iomanip>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/files.h>
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
	  const char *inputFileName = "plain.txt";
	  const char *cipherFileName = "cipher.dat";
	  const char *outputFileName = "Plain2.txt";

	  // Create text file

	  ofstream plain;
	  plain.open(inputFileName);
	  plain << "I can do it .\n";
	  plain << "I bet you can.\n";
	  plain.close();

	  //Create Key and IV

      AutoSeededRandomPool rnd;
	  SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
	  rnd.GenerateBlock( key, key.size() );

	  byte iv[AES::BLOCKSIZE];
	  rnd.GenerateBlock(iv, AES::BLOCKSIZE);

      //AES Encryption

	  CFB_Mode<AES>::Encryption cfbEncryption(key, key.size(), iv);
	  StreamTransformationFilter *encryptor;
	  encryptor = new StreamTransformationFilter(cfbEncryption,new FileSink(cipherFileName) );
	  encryptor->Put(iv, 16);
	  FileSource(inputFileName, true, encryptor);

      //AES Dencryption

	  CFB_Mode<AES>::Decryption cfbDecryption(key, key.size(), iv);
	  StreamTransformationFilter *decryptor;
	  decryptor = new StreamTransformationFilter(cfbDecryption, new FileSink(outputFileName));

	  char garbage[16], iv_garbage[16];
	  ifstream inf;
	  inf.open(cipherFileName); inf.read(iv_garbage, 16);
	  cfbDecryption.ProcessData((byte *)garbage, (const byte *)iv_garbage,16);
	  FileSource(inf, true, decryptor);
	  inf.close();


    return "This is Aesencryption";
  }

};

class Desencryption
  : public Encrypt
{
public:
  virtual char const * encryptdecrypt()
  {
	  const char *inputFileName = "DES_plain.txt";
	  const char *cipherFileName = "DES_cipher.dat";
	  const char *outputFileName = "DES_Plain2.txt";

	  // Create test plaintext file

	  ofstream plain;
	  plain.open(inputFileName);
	  plain << "I can do it .\n";
	  plain << "I bet you can.\n";
	  plain.close();

	  AutoSeededRandomPool prng;

	  // Create Key and IV

	  SecByteBlock key(0x00, DES_EDE2::DEFAULT_KEYLENGTH);
	  prng.GenerateBlock(key, key.size());

	  byte iv[DES_EDE2::BLOCKSIZE];
	  prng.GenerateBlock(iv, sizeof(iv));

	  //Triple DES Encryption

	  try
	  {

	      CBC_Mode< DES_EDE2 >::Encryption e;
	      e.SetKeyWithIV(key, key.size(), iv);

	      StreamTransformationFilter *encryptor;
	      	  	   encryptor = new StreamTransformationFilter(e,
	      	  	     new FileSink(cipherFileName) );
	      	  	 encryptor->Put(iv, 16);
	      	     FileSource(inputFileName, true, encryptor);
	  }

	  catch(const CryptoPP::Exception& e)
	  {
	      cerr << e.what() << endl;
	      exit(1);
	  }


	  //Triple DES DEncryption

	  try
      {
		  CBC_Mode< DES_EDE2 >::Decryption d;
		  d.SetKeyWithIV(key, key.size(), iv);

		  StreamTransformationFilter *decryptor;

		  decryptor = new StreamTransformationFilter(d, new
	     	  	 FileSink(outputFileName));


		  char garbage[16], iv_garbage[16]; // place for IV stuff

		  ifstream inf;
	      inf.open(cipherFileName); inf.read(iv_garbage, 16); // absorb random prefix

	      d.ProcessData((byte *)garbage, (const byte *)iv_garbage,16);
	      FileSource(inf, true, decryptor);
	      inf.close();

	     }
	  catch(const CryptoPP::Exception& e)
	  {
		  cerr << e.what() << endl;
		  exit(1);
	  }

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
