//
// Created by prang on 25.09.2024.
//

#include "EmailUtils.h"
#include "../../include/global.h"
#include <iostream>

void EmailUtils::sendEmailWithRust(const std::string& message_body) {
  // Build the command to execute the Rust program, passing the message body as an argument
  std::string command = EMAIL_EXE.string() + " \"" + message_body + "\"";

  // Execute the Rust program
  int result = system(command.c_str());

  // Check the result of the system command
  if (result == 0) {
    std::cout << "Email sent successfully." << std::endl;
  } else {
    std::cerr << "Failed to send email. Error code: " << result << std::endl;
  }
}