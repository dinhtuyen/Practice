
#ifndef FF_DATABASE
#define FF_DATABASE

#include "ff_invfile.h"
#include "ff_sort.h"

#define DB_RANSAC 500


class ff_database: public ff_invfile {
public:
	ff_database(void);
	~ff_database(void);

	// Class initialization and memory allocation
	// Param: nrdocs ... (IN) maximum number of documents in database
	//        nrvisualwords ... (IN) range of vw's quantization of voctree
	//        maxvw ....... (IN) max number of vw per document
	int init(int nrdocs, int nrvisualwords, int maxvw);

	// Inserts a document into database. "docname" is a label for the document. On query the label gets returned.
	// Param: vw ... (IN) visual words of the document
	//        nr ... (IN) length of visual word vector
	//        docname ... (IN) label for document
	int insertdoc(unsigned int *vw, int nr, int docname);
	int insertdoc(unsigned int *vw, int nr, int docname, float *x, float *y);

	int match(int docpos, unsigned int *vw, int nr, float *x, float *y, float *x1, float *y1, float *x2, float *y2);
	int getforwarddata(int docpos, float *x, float *y, float *s);
	void normalize(void);

	void clean(void);

private:
	int m_init;
	unsigned int *m_vw;
	int *m_nrvw;
	float *m_x;
	float *m_y;
	float *m_s;
	float *m_X;
	float *m_Y;
	float *m_Z;
	unsigned char *m_sift;
	int *m_blockpt;
	int m_blockcount;
	ff_heapsort m_vwsorter;
	float **m_svdU;
	float *m_svdW;
	float **m_svdV;
	float **m_Harr;
	float **m_ptarr;
	float *m_gscore;

};

#endif //FF_DATABASE