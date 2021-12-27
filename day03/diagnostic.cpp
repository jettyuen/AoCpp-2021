#include "diagnostic.hpp"
#include <iostream>

Diagnostic::Diagnostic(std::vector<std::string> binaries)
    : _diagnosticReport{std::move(binaries)} {
  analyze();
}
void Diagnostic::setPcRates(const std::vector<std::string> &binaries,
                            std::string &gRate, std::string &eRate) {
  const size_t lineLength{binaries.at(0).length()};
  const size_t lineCount{binaries.size()};
  std::vector<int> zeroBitCounts(lineLength);
  std::fill(zeroBitCounts.begin(), zeroBitCounts.end(), 0);
  for (auto col{0}; col < lineLength; col++) {
    for (auto row{0}; row < binaries.size(); row++) {
      if (binaries.at(row).at(col) == '0') {
        zeroBitCounts.at(col) += 1;
      }
    }
  }
  for (auto &num: zeroBitCounts) {
    if (num > lineCount / 2) {
      gRate.append("0");
      eRate.append("1");
    } else {
      gRate.append("1");
      eRate.append("0");
    }
  }
}
void Diagnostic::setLsRates() {
  // set pc rates every iteration
  // use gamma binary for oxy because gamma bit is 1 if they are equal.
  // iteration that says, while diagnostic report length is greater than 1, loop
  std::vector<std::string> oBins{}, cBins{};
  std::vector<std::string> report(_diagnosticReport);
  const size_t binLength{report[0].length()};
  std::string gRate{_gRate}, eRate{_eRate};
  while (report.size() > 1) {
    for (auto col{0}; col < binLength; col++) {
      oBins.clear();
      for (auto row{0}; row < report.size(); row++) {
        if ((gRate[col] == '0' && report[row][col] == '0') ||
            (gRate[col] == '1' && report[row][col] == '1')) {
          oBins.push_back(report[row]);
        }
      }
      gRate.clear();
      eRate.clear();
      report = oBins;
      setPcRates(report, gRate, eRate);
    }
  }
  _oRate = std::string(report[0]);
  gRate = _gRate;
  eRate = _eRate;
  std::vector<std::string> newReport(_diagnosticReport);
  while (newReport.size() > 1) {
    for (auto col{0}; col < binLength; col++) {
      cBins.clear();
      for (auto row{0}; row < newReport.size(); row++) {
        if ((gRate[col] == '0' && newReport[row][col] == '1') ||
            (gRate[col] == '1' && newReport[row][col] == '0')) {
          cBins.push_back(newReport[row]);
        }
      }
      if (!cBins.empty()) {
        newReport = cBins;
      } else {
        break;
      }
      gRate.clear();
      eRate.clear();
      setPcRates(newReport, gRate, eRate);
    }
  }
  _cRate = std::string(newReport[0]);
}
void Diagnostic::analyze() {
  setPcRates(_diagnosticReport, _gRate, _eRate);
  setLsRates();
}
int Diagnostic::getPc() {
  int gRate = rateToDecimal(_gRate);
  int eRate = rateToDecimal(_eRate);
  return gRate * eRate;
}
int Diagnostic::getLc() {
  int oRate = rateToDecimal(_oRate);
  int cRate = rateToDecimal(_cRate);
  return oRate * cRate;
}
int Diagnostic::rateToDecimal(const std::string &rate) {
  return std::stoi(rate, nullptr, 2);
}
void Diagnostic::report() {
  int powerConsumption = getPc();
  int lifeSupportRating = getLc();
  std::cout << "Gamma Rate: " << _gRate << std::endl;
  std::cout << "Epsilon Rate: " << _eRate << std::endl;
  std::cout << "Power Consumption: : " << powerConsumption << std::endl;
  std::cout << "\nOxygen Generator Rate: " << _oRate << std::endl;
  std::cout << "CO2 Scrubber Rate: " << _cRate << std::endl;
  std::cout << "Life Support Rating: " << lifeSupportRating << std::endl;
}
