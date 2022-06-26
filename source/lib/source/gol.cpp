#include "gol.hpp"

benlib::Gol::Gol(const uint64_t width, const uint64_t height)
{
  std::vector<std::vector<uint8_t>> new_grid(width, std::vector<uint8_t>(height, 0));
  grid = new_grid;
}

benlib::Gol::Gol(uint8_t** _grid, const uint64_t width, const uint64_t height)
{
  std::vector<std::vector<uint8_t>> new_grid(width, std::vector<uint8_t>(height, 0));
  for (uint64_t i = 0; i < width; i++) {
    for (uint64_t j = 0; j < height; j++) {
      new_grid[i][j] = _grid[i][j];
    }
  }
  this->grid = new_grid;
}

benlib::Gol::Gol(bool** _grid, const uint64_t width, const uint64_t height)
{
  std::vector<std::vector<uint8_t>> new_grid(width, std::vector<uint8_t>(height, 0));
  for (uint64_t i = 0; i < width; i++) {
    for (uint64_t j = 0; j < height; j++) {
      new_grid[i][j] = _grid[i][j] ? 1 : 0;
    }
  }
  this->grid = new_grid;
}

benlib::Gol::Gol(const std::vector<std::vector<uint8_t>>& _grid)
{
  this->grid = _grid;
}

benlib::Gol::~Gol() {}

uint64_t benlib::Gol::GetWidth() const
{
  return grid.size();
}

uint64_t benlib::Gol::GetHeight() const
{
  return grid[0].size();
}

void benlib::Gol::Resize(const uint64_t width, const uint64_t height)
{
  // If old grid is same size as new grid, do nothing
  if (width == GetWidth() && height == GetHeight())
    return;

  std::vector<std::vector<uint8_t>> new_grid(width, std::vector<uint8_t>(height, 0));
  for (uint64_t i = 0; i < width; i++) {
    for (uint64_t j = 0; j < height; j++) {
      if (i < grid.size() && j < grid[0].size()) {
        new_grid[i][j] = grid[i][j];
      } else {
        new_grid[i][j] = 0;
      }
    }
  }
  Clear();
  grid = new_grid;
}

void benlib::Gol::Reset()
{
  Fill(0);
}

uint64_t benlib::Gol::GetLivingCells() const
{
  uint64_t livingCell = 0;
  for (uint64_t x = 0; x < grid.size(); x++) {
    for (uint64_t y = 0; y < grid[0].size(); y++) {
      if (grid[x][y]) {
        ++livingCell;
      }
    }
  }
  return livingCell;
}

uint64_t benlib::Gol::GetDeadCells() const
{
  uint64_t deadCell = 0;
  for (uint64_t x = 0; x < grid.size(); x++) {
    for (uint64_t y = 0; y < grid[0].size(); y++) {
      if (!grid[x][y]) {
        ++deadCell;
      }
    }
  }
  return deadCell;
}

uint64_t benlib::Gol::GetCells() const
{
  return grid.size() * grid[0].size();
}

uint64_t benlib::Gol::GetGenerations() const
{
  return generations;
}

void benlib::Gol::SetGenerations(const uint64_t _generations)
{
  this->generations = _generations;
}

std::vector<std::vector<uint8_t>> benlib::Gol::GetGrid() const
{
  return grid;
}

void benlib::Gol::SetGrid(const std::vector<std::vector<uint8_t>>& _grid)
{
  this->grid = _grid;
  // this->grid.clear();
  // this->grid.shrink_to_fit();
  // this->grid.insert(this->grid.end(), grid.begin(), grid.end());
}

void benlib::Gol::SetGrid(const uint8_t** _grid, const uint64_t width, const uint64_t height)
{
  // this->grid.clear();
  // this->grid.shrink_to_fit();
  this->grid.resize(width);
  for (uint64_t i = 0; i < width; i++) {
    this->grid[i].resize(height);
    for (uint64_t j = 0; j < height; j++) {
      this->grid[i][j] = _grid[i][j];
    }
  }
}

void benlib::Gol::SetCell(const uint64_t x, const uint64_t y, const uint8_t alive)
{
  grid[x][y] = alive;
}

uint8_t benlib::Gol::GetCell(const uint64_t x, const uint64_t y) const
{
  return grid[x][y];
}

void benlib::Gol::Print()
{
  for (uint64_t x = 0; x < grid.size(); x++) {
    for (uint64_t y = 0; y < grid[0].size(); y++) {
      if (grid[x][y] == 1) {
        std::cout << " O ";
      } else {
        std::cout << " . ";
      }
      if (y == grid[0].size() - 1) {
        std::cout << "\n";
      }
    }
  }
  std::cout << std::endl;
}

void benlib::Gol::Update()
{
  generations++;

  std::vector<std::vector<uint8_t>> gridB {};
  // Copy grid to gridB
  gridB = grid;
#if defined(_OPENMP)
#  pragma omp parallel for collapse(2) schedule(auto)
#endif
  for (uint64_t x = 0; x < grid.size(); x++) {
    for (uint64_t y = 0; y < grid[0].size(); y++) {
      // Count living neighbors
      uint64_t aliveCell = 0;
      for (int8_t i = -1; i < 2; i++) {
        for (int8_t j = -1; j < 2; j++) {
          // If is not the center cell
          if (i == 0 && j == 0) {
            continue;
          }

          // Avoid underflow of the grid
          if ((x == 0 && i == -1) || (y == 0 && j == -1)) {
            continue;
          }

          // Avoid overflow of grid
          if (x + i >= grid.size() || y + j >= grid[0].size()) {
            continue;
          }
          // Count living neighbors
          if (gridB[x + i][y + j]) {
            ++aliveCell;
          }
        }
      }
      // Game of life rules
      if (grid[x][y]) {
        if (aliveCell < 2 || aliveCell > 3) {
          grid[x][y] = 0;
        }
      } else {
        if (aliveCell == 3) {
          grid[x][y] = 1;
        }
      }
    }
  }
}

void benlib::Gol::Clear()
{
  grid.clear();
  grid.shrink_to_fit();
}

void benlib::Gol::RandomFill()
{
  std::mt19937_64 rng;
  std::random_device rnd_device;
  rng.seed(rnd_device());
  std::uniform_int_distribution<uint32_t> dist(0, 1000);
  auto gen = [&dist, &rng]() { return dist(rng) < 500 ? 1 : 0; };

  for (auto& i : grid) {
    std::generate(i.begin(), i.end(), gen);
  }
}

void benlib::Gol::RandomFill(std::mt19937_64& rng)
{
  std::uniform_int_distribution<uint32_t> dist(0, 1000);
  auto gen = [&dist, &rng]() { return dist(rng) < 500 ? 1 : 0; };

  for (auto& i : grid) {
    std::generate(i.begin(), i.end(), gen);
  }
}

void benlib::Gol::RandomFill(const float percentage)
{
  std::mt19937_64 rng;
  std::random_device rnd_device;
  rng.seed(rnd_device());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  auto gen = [&dist, &rng, &percentage]() { return dist(rng) < percentage ? 1 : 0; };

  for (auto& i : grid) {
    std::generate(i.begin(), i.end(), gen);
  }
}

void benlib::Gol::Fill(const uint8_t value)
{
  for (auto& i : grid) {
    std::fill(i.begin(), i.end(), value);
  }
}

uint8_t benlib::Gol::operator==(const benlib::Gol& other) const
{
  return grid == other.grid;
}

uint8_t benlib::Gol::operator!=(const benlib::Gol& other) const
{
  return grid != other.grid;
}

benlib::Gol& benlib::Gol::operator=(const benlib::Gol& other)
{
  // if (this == &other)
  //     return *this;
  grid = other.grid;
  return *this;
}

uint8_t benlib::Gol::operator()(const uint64_t x, const uint64_t y) const
{
  return grid[x][y];
}

void benlib::Gol::Deserialize(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cout << "Could not open file " << filename << std::endl;
    return;
  }

  std::string line;
  std::vector<std::vector<uint8_t>> _grid {};
  while (std::getline(file, line)) {
    // std::cout << line << std::endl;
    std::vector<uint8_t> row {};
    for (char c : line) {
      if (c == '0') {
        row.push_back(1);
      } else if (c == '.') {
        row.push_back(0);
      } else if (c == '\n') {
      } else {
        // std::cout << "Unknown character: " << c << std::endl;
      }
    }
    _grid.push_back(row);
  }
  file.close();
  this->Clear();
  this->grid = _grid;
}

void benlib::Gol::Serialize(const std::string& filename)
{
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cout << "Could not open file " << filename << std::endl;
    return;
  }

  for (uint64_t x = 0; x < grid.size(); x++) {
    for (uint64_t y = 0; y < grid[0].size(); y++) {
      if (grid[x][y]) {
        file << "0";
      } else {
        file << ".";
      }
    }
    file << "\n";
  }
  file.close();
}
