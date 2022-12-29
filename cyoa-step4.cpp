#include <cstdlib>
#include <fstream>
#include <string>

#include "cyoa.hpp"

int main(int argc, char ** argv) {
  if (argc == 1) {
    std::cerr << "Usage: ./cyoa-step4 __filename__\n";
    return EXIT_FAILURE;
  }
  else if (argc > 2) {
    std::cerr << "Usage: ./cyoa-step4 __filename__\n";
    return EXIT_FAILURE;
  }
  string path = string(argv[1]);
  Story * story;
  try {
    story = createStory(path);
    story->validateStory();
    story->playStory();
  }
  catch (std::invalid_argument & e) {
    std::cerr << e.what();
    return EXIT_FAILURE;
  }

  delete story;
  return EXIT_SUCCESS;
}
