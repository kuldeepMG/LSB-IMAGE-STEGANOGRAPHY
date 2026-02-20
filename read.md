============================================================
🔐 PROJECT NAME  : LSB Steganography
📅 DURATION      : Dec 2025 - Dec 2025
🏫 INSTITUTE     : Emertxe Information Technologies
💻 LANGUAGE      : C Programming
============================================================

📌 DESCRIPTION
------------------------------------------------------------
• Implemented Least Significant Bit (LSB) Steganography
  to hide and extract secret text inside BMP images.
• Ensures minimal visual distortion of the image.
• Secure encoding and decoding using bit-level operations.

🎯 OBJECTIVE
------------------------------------------------------------
• Demonstrate data hiding using bitwise manipulation.
• Maintain image integrity while embedding secret data.

🛠 TECHNOLOGIES USED
------------------------------------------------------------
✔ C Programming
✔ File Handling (fopen, fread, fwrite, fclose)
✔ Bitwise Operations (&, |, <<, >>)
✔ BMP Image Format Handling
✔ Structures
✔ Command Line Arguments
✔ Dynamic Memory Allocation
✔ Modular Programming

✨ FEATURES
------------------------------------------------------------
✔ Encode secret text inside BMP image
✔ Decode hidden text from stego image
✔ Magic String validation ("#*")
✔ Encode file extension
✔ Encode secret file size (32-bit)
✔ Capacity checking before encoding
✔ Copy 54-byte BMP header
✔ Minimal image distortion
✔ Simple CLI interface

⚙ WORKING PRINCIPLE
------------------------------------------------------------
➤ Each character is converted into 8 bits.
➤ Each bit is stored in the LSB of image bytes.
➤ 8 image bytes store 1 character.
➤ Magic string ensures correct decoding.
➤ During decoding, LSBs are extracted and
  reconstructed to original message.

============================================================
