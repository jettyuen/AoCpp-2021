#include <string>
#include <vector>

class Diagnostic {
  public:
  void report();

  explicit Diagnostic(std::vector<std::string> binaries);

  private:
  std::string _gRate{}, _eRate{}, _oRate{}, _cRate{};
  std::vector<std::string> _diagnosticReport;

  static void setPcRates(const std::vector<std::string> &binaries, std::string &gRate,
                  std::string &eRate);
  void setLsRates();
  void analyze();
  static int rateToDecimal(const std::string& rate);
  int getPc();
  int getLc();
};