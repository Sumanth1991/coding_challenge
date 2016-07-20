//#include <iostream>
//
// #include <cryptopp/aes.h>
// #include <cryptopp/modes.h>
// #include <cryptopp/sha.h>
// #include <cryptopp/osrng.h>
// #include <cryptopp/files.h>
// #include <cryptopp/filters.h>
//
// USING_NAMESPACE(CryptoPP);
// USING_NAMESPACE(std);
//
// /**************************************************************/
//   /* Code to illustrate usage of cryptographic primitives     */
//   /* provided by Crypto++ library v5.5.1 of Wei Dai.          */
//   /*                                                          */
//   /* In particular, the program provides two functions:       */
//   /*                                                          */
//   /* 1. Encrypt a file (key based on password) with random IV;*/
//   /*                                                          */
//   /* 2. Decrypt previously encrypted file.                    */
//   /*                                                          */
//   /* Issue with the original code:                            */
//   /*     - set IV in the encryptor but also used it as random */
//   /*       prefix, so garbage was the 1st encrypted block;    */
//   /*     - did not "fast-forward" the encrypted file past the */
//   /*       first block (which was encrypted random garbage).  */
//   /*                                                          */
//   /* Corrections performed:                                   */
//   /*     - fixed encryptor by leaving random block prefixed,  */
//   /*       but removing redundancy (having it in encryptor);  */
//   /*     - fixed decryptor (see the source code).             */
//   /*                                                          */
//   /*==========================================================*/
//   /* Written by:                                              */
//   /*                   Rash <[hidden email]>            */
//   /*                                                          */
//   /* Modified by:                                             */
//   /*                   Mouse <[hidden email]>        */
//   /*                   Sun Sep 2, 12:25:00 2007               */
//   /*                                                          */
//   /************************************************************/
//   /*                                                          */
//   /* Copyright (C) 2007 Rash  <[hidden email]>          */
//   /* Copyright (C) 2007 Mouse <[hidden email]>       */
//   /*                                                          */
//   /************************************************************/
//
// void encryptFile(const char* password, const char* inputFileName,
// const char* outputFileName)
// {
//   byte pass[AES::BLOCKSIZE];	// digest of password
//   byte iv[16];	// Initial Vector (IV),  misused
//                                 // by original author
//   byte true_iv[16];             // real IV used - set to zero
//
//   AutoSeededRandomPool rng;
//
//   // digest password
//   StringSource(password, true,
//       new HashFilter(*(new SHA256),
//      new ArraySink(pass, AES::BLOCKSIZE)));
//
//   // random Initial Vector
//   rng.GenerateBlock(iv, 16);
//   memset(true_iv, 0, 16);
//
//   // create object for encrypting
//   AES::Encryption aesEncryption(pass,
// CryptoPP::AES::DEFAULT_KEYLENGTH);
//   CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption,
//    true_iv);
//
//   StreamTransformationFilter *encryptor;
//   encryptor = new StreamTransformationFilter(cbcEncryption,
//     new FileSink(outputFileName) );
//
//   encryptor->Put(iv, 16); // this prefixes the file with random block (not IV)
//                  // Cryptographically it is equivalent to IV, so just as good
//
//   // "bind" a file and encrypt one
//   FileSource(inputFileName, true, encryptor);
// }
//
//
// void decryptFile(const char* password, const char*inputFileName,
// const char* outputFileName)
// {
//   byte pass[AES::BLOCKSIZE];
//   byte iv[16];        // here's 1st problem: AES IV is 16 bytes
//   byte head_file[16]; // so must skip 16 bytes, not 8.
//
//   memset(iv, 0, 16); // very correct - in fact the encryptor prefixes file
//   // with a random block, so no need to pass the IV explicitly.
//
//   try {
//     StringSource(password, true, new HashFilter(*(new SHA256), new
// ArraySink(pass,AES::BLOCKSIZE)));
//
//     CryptoPP::AES::Decryption aesDecryption(pass,
//    CryptoPP::AES::DEFAULT_KEYLENGTH);
//     CryptoPP::CBC_Mode_ExternalCipher::Decryption
// cbcDecryption(aesDecryption, iv);
//
//     StreamTransformationFilter *decryptor;
//     decryptor = new
//       StreamTransformationFilter(cbcDecryption, new
// FileSink(outputFileName));
//
//
//     // decryptor->Get(head_file, 16); // does not do anything useful, wrong here
//     // We must somehow decrypt 1st block of the input file, without sending the
//     // result into the output file.
//     char garbage[16], iv_garbage[16]; // place for IV stuff
//     ifstream inf;
//     inf.open(inputFileName); inf.read(iv_garbage, 16); // absorb random prefix
//
//     // Decrypt random prefix (with zero IV) to some dummy buffer to get
//     // (a) decryptor state adjusted to IV, and
//     // (b) file position pointer advanced to the past-IV spot.
//     cbcDecryption.ProcessData((byte *)garbage, (const byte *)iv_garbage,
// 16);
//
//     // NOW can run the decryption engine in "automatic" mode
//     FileSource(inf, true, decryptor);
//
//     inf.close(); // to be nice
//   }
//   catch(CryptoPP::Exception &e)
//     {
//       cerr << "Caught exception during decryption!" << endl;
//       return;
//     }
// }
//
// int main(int argc, char *argv[])
// {
//   const char *pwd  = "not-so-complicated parole";
//   const char *pfn = "plain.txt";
//   const char *cfn = "cipher.dat";
//   const char *ofn = "plain2.txt";
//
//   // Create test plaintext file "plain.txt"
//   ofstream plain;
//   plain.open(pfn);
//   plain << "An excellent test of file encryption techniques.\n";
//   plain << "This test will not demonstrate many tricks though.\n";
//   plain.close();
//
//   // Invoke encryptor to encrypt into "cipher.dat"
//   encryptFile(pwd, pfn, cfn);
//
//   // Invoke decryptor to decrypt into "plain2.txt"
//   decryptFile(pwd, cfn, ofn);
//
//   exit(0);
// }
