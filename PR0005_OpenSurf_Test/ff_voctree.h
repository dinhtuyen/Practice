
#ifndef FF_VOCTREE
#define FF_VOCTREE
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

class ff_voctree {
public:
	ff_voctree(void);
	~ff_voctree(void);
	
	// Class initialization, memory allocation and voctree read from file
	// Param: fname ... (IN) path to voctree
	int init(const char *fname, const int truncate = 0);

	// Memory deallocation
	void clean(void);

	// Quantize a 128 dimensional SIFT vector using the voctree. Distances are computed in L2 norm.
	// Param: vwi ... (OUT) visual word for the SIFT vector
	//        sift... (IN) input SIFT vector 128 chars normalized to 0..255
	void quantize(unsigned int *vwi, unsigned char *desc) const;
	// Computes the number of treenodes (clustercenters) for given split and level
	int calcnrcenters(int splits, int levels);

	// Returns number of levels for currently loaded voctree
	inline int nrlevels(void) { return m_levels; };
	// Returns number of splits for currently loaded voctree
	inline int nrsplits(void) { return m_splits; };
	// Returns number of centers for currently loaded voctree
	inline int nrcenters(void) { return m_nrcenters; };
	// Returns number of vw's for current loaded voctree
	inline int nrvisualwords(void) { return m_visualwords; };

	///
	inline int isvalid(void) {return m_init;}
private:
	int m_init;
	unsigned char *m_voc;
	unsigned char *m_cellinfo;
	unsigned int *m_vwtable;
	unsigned int m_levels;
	unsigned int m_splits;
	unsigned int m_nrcenters;
	unsigned int m_visualwords;
	unsigned int *m_pInterNodes;
	int   m_half; 
	__int64 m_centerpos;
	double m_mindist;
	int m_minindex;
	double m_dist;
	int m_previndex;
	int m_offset;

};

#endif //FF_VOCTREE