#include <cstdlib>
#include <fstream>
#include <string>
 
#include "cyoa.hpp"

int main(int argc, char ** argv) {
  if (argc == 1) {
    std::cerr << "Usage: ./cyoa-step3 __filename__\n";
    return EXIT_FAILURE;
  }
  else if (argc > 2) {
    std::cerr << "Usage: ./cyoa-step3 __filename__\n";
    return EXIT_FAILURE;
  }
  string path = string(argv[1]);
  vector<Path *> paths;
  Story * story;
  try {
    story = createStory(path);
    story->validateStory();
    paths = story->getCycleFreeWinningPaths();
    if (paths.empty()) {
      std::cout << "This story is unwinnable\n";
    }
    else {
      for (size_t i = 0; i < paths.size(); i++) {
        paths[i]->print();
      }
    }
  }
  catch (std::invalid_argument & e) {
    std::cerr << e.what();
    return EXIT_FAILURE;
  }
  for (size_t i = 0; i < paths.size(); i++) {
    delete paths[i];
  }

  delete story;
  return EXIT_SUCCESS;
}
