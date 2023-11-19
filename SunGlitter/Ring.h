#ifndef _RING__H_
#define _RING__H_

class Ring {

  private:
    int* m_ring;
    int m_sum;
    byte m_len;
    byte m_curr;
    
  public:
    Ring(const byte n) :
    m_ring(new int[n]), m_sum(0), m_len(n), m_curr(0) {
      int* p = m_ring;
      for (int i = 0; i < m_len; ++i) {
        *p++ = 0;
      }
   }

    int update(const int val) {
      m_sum += (val - m_ring[m_curr]);
      m_ring[m_curr] = val;
      m_curr = (m_curr + 1) % m_len;
      return m_sum / m_len;
    }
  
};

#endif
