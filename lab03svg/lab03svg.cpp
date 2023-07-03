#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const char GIST_CHAR = '*';
const char GIST_SPACE = ' ';
const size_t SCREEN_WIDTH = 80;
const size_t MAX_ASTERISK = SCREEN_WIDTH - 3 - 1;

void svg_text(double left, double baseline, string text);
void svg_begin(double width, double height);
void svg_end();
void svg_rect(double x, double y, double width, double height, string stroke, string fill);
void show_histogram_svg(const vector<size_t>& bins);
vector<double> getNumbersForBins(size_t numbersCount);
vector<size_t> binFull(vector<double> numbers, size_t binCount);
void findMinMax(vector<double> numbers, double& min, double& max);
size_t findMaxBinCount(vector<size_t> bins);

int main() {
    size_t numbersCount;
    cerr << "Enter numbers count: ";
    cin >> numbersCount;

    vector<double> numbers;
    cerr << "Enter numbers: ";
    numbers = getNumbersForBins(numbersCount);

    size_t binCount;
    cerr << "Enter bins count: ";
    cin >> binCount;
    cerr << endl;
    vector<size_t> bins = binFull(numbers, binCount);
    size_t maxBinCount = findMaxBinCount(bins);//корзина  с большим количеством элементов
    //showHistogram(bins, maxBinCount, binCount);
    show_histogram_svg(bins);
}

//+
vector<double> getNumbersForBins(size_t numbersCount) {
    srand(time(NULL));
    vector<double> numbers(numbersCount);
    for (int i = 0; i < numbersCount; i++) {
        cin >> numbers[i];
    }
    return numbers;
}
//+
vector<size_t> binFull(vector<double> numbers, size_t binCount) {
    vector<size_t> bins(binCount);
    double min = numbers[0];
    double max = numbers[0];
    findMinMax(numbers, min, max);
    double step = (max - min) / binCount;

    for (size_t x : numbers) {
        bool found = false;
        for (size_t i = 0; (i < binCount - 1) && !found; i++) {
            auto lo = min + i * step;
            auto hi = min + (i + 1) * step;
            if ((lo <= x) && (x < hi)) {
                bins[i]++;
                found = true;
            }
        }
        if (!found) {
            bins[binCount - 1]++;
        }
    }
    return bins;
}

void findMinMax(vector<double> numbers, double& min, double& max) {
    for (int i = 1; i < numbers.size(); i++) {
        if (numbers[i] < min) {
            min = numbers[i];
        }
        else if (numbers[i] > max) {
            max = numbers[i];
        }
    }
}

size_t findMaxBinCount(vector<size_t> bins) {
    size_t mx = 0;
    for (size_t item : bins) {
        if (item > mx) {
            mx = item;
        }
    }
    return mx;

}

void svg_begin(double width, double height) {
    cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    cout << "<svg ";
    cout << "width='" << width << "' ";
    cout << "height='" << height << "' ";
    cout << "viewBox='0 0 " << width << " " << height << "' ";
    cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void svg_end() {
    cout << "</svg>\n";
}

void svg_text(double left, double baseline, string text) {
    cout << "<text x='" << left
        << "' y=' " << baseline << "'>"
        << text << "</text>";
}

void svg_rect(double x, double y, double width, double height, string stroke, string fill) {
    cout << "<rect x='" << x
        << "' y=' " << y << "' width='"
        << width << "' height='"
        << height << "' stroke='"
        << stroke << "' fill='"
        << "' />";
}

void show_histogram_svg(const vector<size_t>& bins) {
    const auto IMAGE_WIDTH = 400;
    const auto IMAGE_HEIGHT = 300;
    const auto TEXT_LEFT = 20;
    const auto TEXT_BASELINE = 20;
    const auto TEXT_WIDTH = 50;
    const auto BIN_HEIGHT = 30;
    const auto BLOCK_WIDTH = 10;
    const auto RECT_STROKE = "pink";
    const auto RECT_FILL = "#ffeeee";

    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);
    double top = 0;
    for (size_t bin : bins) {
        const double bin_width = BLOCK_WIDTH * bin;
        svg_text(TEXT_LEFT, top + TEXT_BASELINE, to_string(bin));
        svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT, RECT_STROKE, RECT_FILL);
        top += BIN_HEIGHT;
    }
    svg_end();
}
