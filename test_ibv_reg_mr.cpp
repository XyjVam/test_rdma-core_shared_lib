#include "ib_context.hpp"

#include <cstdio>
#include <memory>
#include <string>


using std::string;

int main()
{
  ddf::ib_context ibc;
  //------------------------------------------
  const int datasz = 100;
  auto pdata = std::make_unique<char[]>(datasz);
  perror("allocate PD");
  auto pd_ = std::shared_ptr<ibv_pd>(ibv_alloc_pd(ibc.get_ctx())
      , [](ibv_pd* p)
      {
        if(p)
        {
          if(ibv_dealloc_pd(p))
          {
            perror("Failed to deallocate PD");
          }
        }
        perror("PD was deallocated");
      }
      );
  if(!pd_)
  {
    perror("'ibv_alloc_pd' failed");
    return -1;
  }
  //Create the completion queue
  perror("Create CQ");
  int cq_size = 100;
  auto cq_ = std::shared_ptr<ibv_cq>(
      ibv_create_cq(ibc.get_ctx(), cq_size, NULL, NULL, 0)
      ,[](ibv_cq* cq)
      {
        if(cq)
          if(ibv_destroy_cq(cq))
            perror("Failed to destroy CQ");
        perror("CQ was destroyed");
      }
      );
  if(!cq_)
  {
    perror("'ibv_create_cq' failed");
    return -1;
  }
  //Register the memory region
  perror("Register MR");
  int mr_flags = 0;
  mr_flags = IBV_ACCESS_LOCAL_WRITE;
  auto mr_data_ = std::shared_ptr<ibv_mr>(
      ibv_reg_mr(pd_.get()
        , pdata.get()
        , datasz
        , mr_flags)
      , [](ibv_mr* mr)
      {
        if(ibv_dereg_mr(mr))
          perror("Failed to deregister data MR");
        perror("MR data was deregistered");
      }
      );
  if(!mr_data_)
  {
    perror("'ibv_reg_mr' for data failed");
    return -1;
  }
  perror("MR was registered");

  return 0;
}
