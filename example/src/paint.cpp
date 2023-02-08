#include "paint.hpp"

Paint::Image::Image() : BM(nullptr) {}

Paint::Image::Image(const Image& image) {
    this->BFH = image.BFH;
    this->BIH = image.BIH;
    this->BM = new BGR*[image.BIH.height];
    for (int i = 0; i < image.BIH.height; i++) {
        BM[i] = new BGR[image.BIH.width];
        for (int j = 0; j < image.BIH.width; j++) BM[i][j] = image.BM[i][j];
    }
}

Paint::Image::~Image() {
    if (this->BM) {
        for (int i = 0; i < BIH.height; i++) delete[] BM[i];
        delete[] BM;
    }
}

Paint::Image& Paint::Image::operator=(const Image& image) {
    if (this->BM) {
        for (int i = 0; i < BIH.height; i++) delete[] BM[i];
        delete[] this->BM;
    }

    this->BFH = image.BFH;
    this->BIH = image.BIH;
    this->BM = new BGR*[image.BIH.height];
    for (int i = 0; i < image.BIH.height; i++) {
        BM[i] = new BGR[image.BIH.width];
        for (int j = 0; j < image.BIH.width; j++) BM[i][j] = image.BM[i][j];
    }
    return *this;
}

bool Paint::Image::load(std::string path) {
    std::ifstream file(path, std::ios_base::binary);

    if (!file.bad()) {
        file.read((char*)&BFH, sizeof(BitmapFileHeader));
        if ((BFH.signature != 19778) || (BFH.reserved1 != 0) || (BFH.reserved2 != 0)) {
            std::cout << "Файл не поддерживается!\n";
            file.close();
            return false;
        }

        file.read((char*)&BIH, sizeof(BitmapInfoHeader));
        if ((BIH.headerSize != 40) || (BIH.bitsPerPixel != 24) || (BIH.colorsInColorTable != 0) || (BIH.importantColorCount != 0)) {
            std::cout << "Файл не поддерживается!\n";
            file.close();
            return false;
        }

        char temp;
        BM = new BGR*[BIH.height];
        for (int i = 0; i < BIH.height; i++) {
            BM[i] = new BGR[BIH.width];
            file.read((char*)BM[i], BIH.width * sizeof(BGR));
            for (int i = 0; i < (4 - (BIH.width * 3) % 4) % 4; i++) file.read(&temp, sizeof(char));
        }

        file.close();
        return true;
    }
    std::cout << "Не верный путь до файла!\n";
    return false;
}

bool Paint::Image::save(std::string path) {
    if (!BM) throw std::invalid_argument("# Image.save: Empty BitMap!");
    std::ofstream file(path, std::ios_base::binary);

    if (!file.bad()) {
        file.write((char*)&BFH, sizeof(BitmapFileHeader));
        file.write((char*)&BIH, sizeof(BitmapInfoHeader));

        int H = BIH.height;
        int W = BIH.width;

        for (int i = 0; i < H; i++) {
            file.write((char*)BM[i], W * sizeof(BGR));
            char temp = '\0';
            for (int i = 0; i < (4 - (W * 3) % 4) % 4; i++) file.write(&temp, sizeof(char));
        }

        file.close();
        return true;
    }
    std::cout << "Не верный путь до файла!\n";
    return false;
}


std::vector<Paint::Image> Paint::slice_image(const Image& image, int x_lines, int y_lines) {
    if (x_lines < 1 || y_lines < 1) throw std::invalid_argument("# Paint::slice_image: Cut lines should be larger then zero!");

    std::vector<Image> images;
    Image n_image(image);
    int H = n_image.BIH.height /= y_lines;
    int W = n_image.BIH.width /= x_lines;

    for (int i = 0; i < y_lines; i++) {
        for (int j = 0; j < x_lines; j++) {
            n_image.BM = new BGR*[H];
            for (int k = 0; k < H; k++) {
                n_image.BM[k] = new BGR[W];
                for (int l = 0; l < W; l++)  n_image.BM[k][l] = image.BM[i * H + k][j * W + l];
            }
            images.push_back(n_image);
        }
    }
    return images;
}

Paint::Image Paint::set_component(const Image& image, char index, unsigned char value) {
    if (index != 'r' && index != 'g' && index != 'b')
        throw std::invalid_argument("# Paint::set_component: Component name should be 'r', 'g' or 'b'!");

    Image n_image(image);
    for (int i = 0; i < n_image.BIH.height; i++) {
        for (int j = 0; j < n_image.BIH.width; j++) {
            if (index == 'r') n_image.BM[i][j].r = value;
            else if (index == 'g') n_image.BM[i][j].g = value;
            else n_image.BM[i][j].b = value;
        }
    }
    return n_image;
}

Paint::Image Paint::put_circle(const Image& image, int x, int y, int radius, BGR f_color, int border, BGR b_color) {
    if (border < 0 || radius < 0) throw std::invalid_argument("# Paint::put_circle: Border and Radius should be positive!");
    if (border > radius) throw std::invalid_argument("# Paint::put_circle: Border should be lower then Radius!");

    Image n_image(image);
    for (int i = 0; i < n_image.BIH.height; i++) {
        for (int j = 0; j < n_image.BIH.width; j++) {
            int S2 = pow(x - j, 2) + pow(y - i, 2), sR2 = pow(radius - border, 2), R2 = pow(radius, 2);
            if (S2 < sR2) n_image.BM[i][j] = f_color;
            if (S2 > sR2 && S2 < R2) n_image.BM[i][j] = b_color;
        }
    }
    return n_image;
}

Paint::Image Paint::put_square(const Image& image, int x1, int y1, int x2, int y2, int x_d, int y_d) {
    int H = image.BIH.height, W = image.BIH.width;
    if (x1 < 0 || x1 > W || y1 < 0 || y1 > H || x2 < x1 || x2 > W || y2 < y1 || y2 > H)
        throw std::invalid_argument("# Paint::put_square: Square should be inside image!");
    
    if (x2 > W - x_d) x2 = W - x_d;
    if (y2 > H - y_d) y2 = H - y_d;
    if (x1 < -x_d) x1 = -x_d;
    if (y1 < -y_d) y1 = -y_d;
    if (x1 > x2 || y1 > y2) return Image(image);

    Image n_image(image);
    BGR** BM = new BGR*[y2 - y1];
    for (int i = 0; i < y2 - y1; i++) {
        BM[i] = new BGR[x2 - x1];
        for (int j = 0; j < (x2 - x1); j++) BM[i][j] = n_image.BM[i + y1][j + x1];
    }
    for (int i = 0; i < y2 - y1; i++) for (int j = 0; j < x2 - x1; j++) n_image.BM[i + y_d][j + x_d] = BM[i][j];
    return n_image;
}

std::ostream& Paint::operator<<(std::ostream& out, const BitmapFileHeader& bfh) {
    return out << bfh.signature << '\n' << bfh.filesize << '\n' << bfh.reserved1 << '\n' << bfh.reserved2 << '\n' << bfh.pixelArrOffset << '\n';
}

std::ostream& Paint::operator<<(std::ostream& out, const BitmapInfoHeader& bih) {
    out << bih.headerSize << '\n' << bih.width << '\n' << bih.height << '\n' << bih.planes << '\n';
    out << bih.bitsPerPixel << '\n' << bih.compression << '\n' << bih.imageSize << '\n' << bih.xPixelsPerMeter << '\n';
    out << bih.yPixelsPerMeter << '\n' << bih.colorsInColorTable << '\n' << bih.importantColorCount << '\n';
    return out;
}

std::ostream& Paint::operator<<(std::ostream& out, const BGR& bgr) {
    return out << bgr.b << '\n' << bgr.g << '\n' << bgr.r << '\n';
}