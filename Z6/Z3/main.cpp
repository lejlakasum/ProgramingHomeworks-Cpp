#include <iostream>
#include <iomanip>
#include <cstring>
#include <stdexcept>
#include <new>
#include <string>
#include <fstream>

template <typename TipEl>
 class Matrica {
     
     int br_redova, br_kolona;
     TipEl **elementi;
     char ime_matrice;
     static TipEl **AlocirajMemoriju(int br_redova, int br_kolona);
     static void DealocirajMemoriju(TipEl **elementi, int br_redova);
     void KopirajElemente(TipEl **elementi);
     
public:

     Matrica(int br_redova, int br_kolona, char ime = 0);
     Matrica(const Matrica &m);
     Matrica(Matrica &&m);
     ~Matrica() { DealocirajMemoriju(elementi, br_redova); }
     Matrica &operator =(const Matrica &m);
     Matrica &operator =(Matrica &&m);
     template<typename Tip2>
     friend std::istream &operator>>(std::istream &tok, Matrica<Tip2> &m);
     template<typename Tip2>
     friend std::ostream &operator<<(std::ostream &tok, const Matrica<Tip2> &m);
     
     template <typename Tip2>
     friend Matrica<Tip2> operator+(const Matrica<Tip2> &m1, const Matrica<Tip2> &m2);
     template <typename Tip2>
     friend Matrica<Tip2> operator-(const Matrica<Tip2> &m1, const Matrica<Tip2> &m2);
     
     template<typename TipEl2>
    friend Matrica<TipEl2> operator*(Matrica<TipEl2> m1, const Matrica<TipEl2> &m2);
    template<typename TipEl2,typename TipEl3>
    friend Matrica<TipEl2> operator*(TipEl3 broj, Matrica<TipEl2> m);
    template<typename TipEl2, typename TipEl3>
    friend Matrica<TipEl2> operator*(Matrica<TipEl2> m, TipEl3 broj);
    
    
    Matrica<TipEl> &operator+=(const Matrica<TipEl> &m1);
    Matrica<TipEl> &operator-=(const Matrica<TipEl> &m1);
    Matrica<TipEl> &operator*=(const Matrica<TipEl> &m1);
    Matrica<TipEl> &operator*=(TipEl broj);
    
    TipEl &operator () (int i, int j);
    TipEl const operator () (int i, int j) const;
    TipEl *operator[](int indeks);
    TipEl const *operator[](int indeks) const;
    
    operator std::string() const;
    
    //DATOTEKE
    void SacuvajUTekstualnuDatoteku(std::string ime_datoteke) const;
    void SacuvajUBinarnuDatoteku(std::string ime_datoteke) ;
    void ObnoviIzBinarneDatoteke(std::string ime_datoteke);
    void ObnoviIzTekstualneDatoteke(std::string ime_datoteke);
    
 };
 
 //DATOTEKE
 template<typename TipEl>
 void Matrica<TipEl>::ObnoviIzTekstualneDatoteke(std::string ime_datoteke) {
     std::ifstream procitaj(ime_datoteke);
     if(!procitaj) throw std::logic_error ("Trazena datoteka ne postoji");
     Matrica<TipEl> pomocna(*this); //Omogucava da ukoliko dodje do bilo kakvih problema objekat ostane u istom stanju
     DealocirajMemoriju(elementi, br_redova);
     char znak1,znak2; int r(0), k(0);
     while(procitaj>>znak1>>znak2) { 
         if(znak1>='0' && znak1<='9' && znak2==',') if(r==0) k++;
     }
    
     while(procitaj>>znak1) { 
         if(znak1=='\n') r++;
     }
     std::cout<<r<<" "<<k;
         
 
 }
 
 template<typename TipEl>
 void Matrica<TipEl>::ObnoviIzBinarneDatoteke(std::string ime_datoteke) {
     
     std::ifstream procitaj(ime_datoteke, std::ios::binary);
     if(!procitaj) throw std::logic_error ("Trazena datoteka ne postoji");
     Matrica<TipEl> pomocna(*this); //Omogucava da ukoliko dodje do bilo kakvih problema objekat ostane u istom stanju
     DealocirajMemoriju(elementi, br_redova);
     procitaj.read(reinterpret_cast<char*>(this), sizeof (*this));
     try {
         AlocirajMemoriju(br_redova, br_kolona);
         for(int i=0; i<br_redova; i++) {
             for(int j=0; j<br_kolona; j++) {
                 procitaj.read(reinterpret_cast<char*>(&elementi[i][j]), sizeof (TipEl));
             }
         }
         if(!procitaj) throw std::logic_error ("Problemi pri citanju datoteke");
     }
     catch(...) {
         DealocirajMemoriju(elementi, br_redova);
         *this=pomocna;
         throw;
     }
 }
 
 template<typename TipEl>
 void Matrica<TipEl>::SacuvajUBinarnuDatoteku(std::string ime_datoteke)  {
     
     std::ofstream upisi(ime_datoteke, std::ios::binary);
     upisi.write(reinterpret_cast<char*>(this), sizeof (*this));
     for(int i=0; i<br_redova; i++) {
         for(int j=0; j<br_kolona; j++)
            upisi.write(reinterpret_cast<char*>(&elementi[i][j]), sizeof (TipEl));
     }
     if(!upisi) throw std::logic_error ("Problemi sa upisom u datoteku");
 }
 
 template<typename TipEl>
 void Matrica<TipEl>::SacuvajUTekstualnuDatoteku(std::string ime_datoteke) const {
     
     std::ofstream upisi(ime_datoteke);
     if(!upisi) throw std::logic_error ("Problemi sa upisom u datoteku");
     for(int i=0; i<br_redova; i++) {
         for(int j=0; j<br_kolona; j++) {
             if(j==br_kolona-1)
                upisi<<elementi[i][j];
            else upisi<<elementi[i][j]<<",";
            if(!upisi) throw std::logic_error ("Problemi sa upisom u datoteku");
         }
         upisi<<std::endl;
     }
 }
 
 
template <typename TipEl>
     TipEl ** Matrica<TipEl>::AlocirajMemoriju(int br_redova, int br_kolona) {
     TipEl **elementi(new TipEl*[br_redova]{});
     try {
        for(int i = 0; i < br_redova; i++) elementi[i] = new TipEl[br_kolona];
    }
     catch(...) {
         DealocirajMemoriju(elementi, br_redova);
     throw;
     }
     return elementi;
 }
 
template <typename TipEl>
 void Matrica<TipEl>::DealocirajMemoriju(TipEl **elementi, int br_redova) {
 for(int i = 0; i < br_redova; i++) delete[] elementi[i];
 delete[] elementi;
 }
 
template <typename TipEl>
 Matrica<TipEl>::Matrica(int br_redova, int br_kolona, char ime) :
 br_redova(br_redova), br_kolona(br_kolona), ime_matrice(ime),
 elementi(AlocirajMemoriju(br_redova, br_kolona)) {}
 
template <typename TipEl>
 void Matrica<TipEl>::KopirajElemente(TipEl **elementi) {
 for(int i = 0; i < br_redova; i++)
 for(int j = 0; j < br_kolona; j++)
 Matrica::elementi[i][j] = elementi[i][j];
 }
 
template <typename TipEl>
 Matrica<TipEl>::Matrica(const Matrica<TipEl> &m) : br_redova(m.br_redova),
 br_kolona(m.br_kolona), ime_matrice(m.ime_matrice),
 elementi(AlocirajMemoriju(m.br_redova, m.br_kolona)) {
 KopirajElemente(m.elementi);
 }
 
template <typename TipEl>
 Matrica<TipEl>::Matrica(Matrica<TipEl> &&m) : br_redova(m.br_redova),
 br_kolona(m.br_kolona), elementi(m.elementi), ime_matrice(m.ime_matrice) {
 m.br_redova = 0; m.elementi = nullptr;
 }
 
 
 //OPERATORI
 
template <typename TipEl>
 Matrica<TipEl> &Matrica<TipEl>::operator =(const Matrica<TipEl> &m) {
 if(br_redova < m.br_redova || br_kolona < m.br_kolona) {
 TipEl **novi_prostor(AlocirajMemoriju(m.br_redova, m.br_kolona));
 DealocirajMemoriju(elementi, br_redova);
 elementi = novi_prostor;
 }
 else if(br_redova > m.br_redova)
 for(int i = m.br_redova; i < br_redova; i++) delete elementi[i];
 br_redova = m.br_redova; br_kolona = m.br_kolona;
 ime_matrice = m.ime_matrice;
 KopirajElemente(m.elementi);
 return *this;
 }
 
template <typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator =(Matrica<TipEl> &&m) {
 std::swap(br_redova, m.br_redova); std::swap(br_kolona, m.br_kolona);
 std::swap(ime_matrice, m.ime_matrice); std::swap(elementi, m.elementi);
 return *this;
 }
 
 template <typename TipEl>
 std::istream &operator>>(std::istream &tok, Matrica<TipEl> &m) {
 for(int i = 0; i < m.br_redova; i++)
 for(int j = 0; j < m.br_kolona; j++) {
 std::cout << m.ime_matrice << "(" << i + 1 << "," << j + 1 << ") = ";
 tok>> m.elementi[i][j];
 }
 return tok;
 }
 
template <typename TipEl>
 std::ostream &operator<<(std::ostream &tok, const Matrica<TipEl> &m)  {
     int sirina_ispisa(tok.width());
 for(int i = 0; i < m.br_redova; i++) {
 for(int j = 0; j < m.br_kolona; j++)
 tok << std::setw(sirina_ispisa) << m.elementi[i][j];
 tok << std::endl;
 }
 return tok;
 }
 
template <typename TipEl>
 Matrica<TipEl> operator+(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
 if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
 throw std::domain_error("Matrice nemaju jednake dimenzije!");
 Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
 for(int i = 0; i < m1.br_redova; i++)
 for(int j = 0; j < m1.br_kolona; j++)
 m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
 return m3;
 }
 
 template <typename TipEl>
 Matrica<TipEl> operator-(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
 if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
 throw std::domain_error("Matrice nemaju jednake dimenzije!");
 Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
 for(int i = 0; i < m1.br_redova; i++)
 for(int j = 0; j < m1.br_kolona; j++)
 m3.elementi[i][j] = m1.elementi[i][j] - m2.elementi[i][j];
 return m3;
 }
 
template<typename TipEl>
Matrica<TipEl> operator*(Matrica<TipEl> m1, const Matrica<TipEl> &m2)  {
    if(m1.br_kolona!=m2.br_redova) throw std::domain_error ("Matrice nisu saglasne za mnozenje");
    Matrica<TipEl> m3(m1.br_redova, m2.br_kolona);
    for(int i=0; i<m1.br_redova; ++i) {
        for(int j=0; j<m2.br_kolona; ++j) {
           double suma(0);
           for(int k=0; k<m1.br_redova; ++k)
                suma+=m1.elementi[i][k]*m2.elementi[k][j];
            m3.elementi[i][j]=suma;
        }
    }
    return m3;
}

template<typename TipEl, typename TipEl2>
Matrica<TipEl> operator*(TipEl2 broj, Matrica<TipEl> m) {
     
     return m*=broj;
}

template<typename TipEl,typename TipEl2>
Matrica<TipEl> operator*(Matrica<TipEl> m, TipEl2 broj) {
    return m*=broj;
}

template<typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator+=(const Matrica<TipEl> &m1){
    if(this->br_redova!=m1.br_redova || this->br_kolona!=m1.br_kolona) throw std::domain_error ("Matrice nemaju jednake dimenzije!");
    for(int i=0; i<this->br_redova; ++i) {
        for(int j=0; j<m1.br_kolona; ++j)
            this->elementi[i][j]+=m1.elementi[i][j];
    }
    return *this;
}

template<typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator-=(const Matrica<TipEl> &m1){
    if(this->br_redova!=m1.br_redova || this->br_kolona!=m1.br_kolona) throw std::domain_error ("Matrice nemaju jednake dimenzije!");
    for(int i=0; i<this->br_redova; ++i) {
        for(int j=0; j<m1.br_kolona; ++j)
            this->elementi[i][j]-=m1.elementi[i][j];
    }
    return *this;
}

template<typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator*=(const Matrica<TipEl> &m1) {
    *this=*this*m1;
    
    return *this;
}

template<typename TipEl>
Matrica<TipEl> &Matrica<TipEl>::operator*=(TipEl broj) {
    for(int i=0; i<this->br_redova; ++i) {
         for(int j=0; j<this->br_kolona; ++j)
            this->elementi[i][j]*=broj;
     }
     return *this;
}

template<typename TipEl>
TipEl &Matrica<TipEl>::operator () (int i, int j) {
        if(i<1 || j<1 || i>4 || j>4) throw std::range_error ("Neispravan indeks");
    return elementi[i-1][j-1];
    }

template<typename TipEl>
TipEl const Matrica<TipEl>::operator () (int i, int j) const {
        if(i<1 || j<1 || i>4 || j>4) throw std::range_error ("Neispravan indeks");
    return elementi[i-1][j-1];
    }
    
template<typename TipEl>
TipEl *Matrica<TipEl>::operator[](int indeks) {
    return &elementi[indeks][0];
}

template<typename TipEl>
TipEl const *Matrica<TipEl>::operator[](int indeks) const {
    return &elementi[indeks][0];
}

template<typename TipEl>
Matrica<TipEl>::operator std::string() const {
    std::string s;
    s.push_back('{');
    for(int i=0; i<br_redova; ++i) {
        s.push_back('{');
        for(int j=0; j<br_kolona; ++j) {
           if(j==br_kolona-1) {
            std::string s2(std::to_string(int(elementi[i][j])));
            for(int k=0; k<s2.length(); ++k) s.push_back(s2[k]);
           }
            else {
                std::string s2(std::to_string(int(elementi[i][j])));
                for(int k=0; k<s2.length(); ++k) s.push_back(s2[k]);
                s.push_back(',');
            }
        }
        if(i==br_redova-1) s.push_back('}');
        else {
            s.push_back('}');
            s.push_back(',');
        }
    }
    s.push_back('}');
    
    return s;
}
 
int main() {
    
    Matrica<double> a(2, 3, 'a');
a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
a[1][0] = 4; a[1][1] = 5; a[1][2] = 7;

a.SacuvajUTekstualnuDatoteku("PROBA.TXT");
a.SacuvajUBinarnuDatoteku("PROBA2.DAT");

a.ObnoviIzBinarneDatoteke("PROBA2.DAT");
a.SacuvajUTekstualnuDatoteku("NOVA.TXT");

a.ObnoviIzTekstualneDatoteke("PROBA.TXT");
 return 0;
}
