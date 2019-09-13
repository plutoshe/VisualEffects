#pragma once

void preProcess(uchar4** h_inputImageRGBA, uchar4** h_outputImageRGBA,
	uchar4** d_inputImageRGBA, uchar4** d_outputImageRGBA,
	unsigned char** d_redBlurred,
	unsigned char** d_greenBlurred,
	unsigned char** d_blueBlurred,
	float** h_filter, int* filterWidth,
	const std::string& filename);
void postProcess(const std::string& output_file, uchar4* data_ptr);
void cleanUp(void);
void generateReferenceImage(std::string input_filename, std::string output_filename);
size_t numRows();
size_t numCols();