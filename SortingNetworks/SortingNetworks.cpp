

#include <cstdio>
#include <vector>
#include <random>
#include <exception>

using namespace std;

struct comparator_network{
  unsigned input_size;
  vector<pair<unsigned, unsigned>> comparators;
  comparator_network(unsigned input_size) : input_size(input_size) {}
  void add_comparator(unsigned a, unsigned b)  { 
    comparators.push_back(pair<unsigned,unsigned>(a,b)); 
  }
  template <class T>
  void apply(vector<T>& v) const {  
    if (v.size()>input_size)
      throw logic_error("can't apply to bigger input");
    auto vsize = v.size();
    for (auto i=comparators.begin(); i!=comparators.end(); i++) {
      auto first = i->first;
      auto second = i->second;
      if (first<vsize && second<vsize)
      if (v[second] < v[first]) {
        std::swap(v[second], v[first]);
      }
    }
  }
  template <class T>
  void apply_reverse(vector<T>& v) const {
    if (v.size()>input_size)          
      throw logic_error("can't apply to bigger input");
    auto vsize = v.size();
    for (auto i=comparators.begin(); i!=comparators.end(); i++) {
      auto first = i->first;
      auto second = i->second;
      if (first<vsize && second<vsize)
      if (v[first] < v[second]) {
        std::swap(v[first], v[second]);
      }
    }
  }
};

comparator_network generate_random_network(unsigned input_size, unsigned comparator_count)
{
  comparator_network a(input_size);
  uniform_int_distribution<int> distribution(0,input_size-1);
  mt19937 mtengine;

  if (input_size >= 2) {
    while(comparator_count--) {
      unsigned first = (distribution(mtengine) + rand())%input_size;
      unsigned second = (distribution(mtengine) + rand())%input_size;
      while (first == second)
        second = (distribution(mtengine) + rand())%input_size;
      a.add_comparator(first,second);
    }
  }
  return a;
}

template <class T>
bool is_sorted(const vector<T> v)
{
  auto i = v.begin();
  auto j = i;
  while (++i!=v.end())
  {
    if (*i < *j)
      return false;
    j = i;
  }
  return true;
}    

template <class T>
bool is_reverse_sorted(const vector<T> v)
{
  auto i = v.begin();
  auto j = i;
  while (++i!=v.end())
  {
    if (*j < *i)
      return false;
    j = i;
  }
  return true;
}

bool is_sorting_network(const comparator_network& network)
{
  if (network.input_size > 32)
    throw domain_error("not supported, largest supported sorting network has 32 inputs");
  unsigned test_count = 1<<network.input_size;
  vector<int> test_vector(network.input_size);
  for(auto test = 0; test<test_count; test++) {
    auto x = test^0x55555555;
    for (auto i=0; i<network.input_size; i++) {
      test_vector[i] = (x&1);
      x>>=1;
    }
    network.apply(test_vector);
    if (!is_sorted(test_vector))
      return false;
  }
  return true;
}

comparator_network find_sorting_network(unsigned input_size, unsigned comparator_count)
{
  for(;;) {
    {
     auto network = generate_random_network(input_size, comparator_count);
     if (is_sorting_network(network)) return network;
    }
  }
  throw logic_error("unreachable");
}

int main(){
  vector<pair<unsigned,unsigned>> to_find;
  to_find.push_back(pair<unsigned,unsigned>(4,5));
  to_find.push_back(pair<unsigned,unsigned>(5,9));
  to_find.push_back(pair<unsigned,unsigned>(6,12));
  to_find.push_back(pair<unsigned,unsigned>(7,16));

  for (auto i=to_find.begin(); i!=to_find.end(); i++)
  {
    auto input_size = i->first;
    auto comparator_count = i->second;
    auto network = find_sorting_network(input_size, comparator_count);
    printf("network for input:%d, comparators:%d\n",input_size, comparator_count);  
    for (auto i=network.comparators.begin(); i!=network.comparators.end(); i++){
      printf("(%d, %d) ",i->first,i->second); 
    }                  
    printf("\n");
    
    for (int y=0; y<network.input_size; y++)
    {
      for (int x=0; x<network.comparators.size(); x++)
      {
        auto first = network.comparators[x].first;
        auto second = network.comparators[x].second;
        if(first == y || second == y) 
          printf("-o-");
        else if(y>first&&y<second||y<first&&y>second)     
          printf("-|-");
        else
          printf("---");
      }            
      printf("\n");
      for (int x=0; x<network.comparators.size(); x++)
      {
        auto first = network.comparators[x].first;
        auto second = network.comparators[x].second;
        if(y>=first&&y<second||y<first&&y>=second)     
          printf(" | ");
        else
          printf("   ");
      }
      printf("\n");
    }   
    printf("\n");
  }

}



