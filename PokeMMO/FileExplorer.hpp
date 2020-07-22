#include "Texture.h"

#include <vector>
#include <string>

#pragma warning(disable : 4996)

class FileExplorer
{
public:
  FileExplorer();

  bool onUpdate();
  void Init();
  const std::string& Explorer();

private:
  void EnterDirectory(const std::string&);

  std::string currpath, selectedfile;

  std::vector<std::string> folders;
  std::vector<std::string> files;

  Texture filetex;
  Texture foldtex;

  bool open;
};
