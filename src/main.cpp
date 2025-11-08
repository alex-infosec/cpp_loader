#include <cstdint>
#include <string>
#include <vector>
#include <cstring>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten/fetch.h>
#include "gator.h"
using namespace emscripten;

static void copyToCanvas(int w, int h, const uint8_t* px, const std::string& id){
  val d = val::global("document");
  val c = d.call<val>("getElementById", val(id));
  c.set("width",  w);
  c.set("height", h);
  val ctx = c.call<val>("getContext", val("2d"));
  val img = ctx.call<val>("createImageData", val(w), val(h));
  val data = img["data"];
  data.call<void>("set", val(typed_memory_view(w*h*4, px)));
  ctx.call<void>("putImageData", img, val(0), val(0));
}

void displayHeaderImage(std::string id){
  std::vector<uint8_t> rgba(gatorWidth * gatorHeight * 4);
  for (int i=0;i<gatorWidth*gatorHeight;i++){
    uint8_t bit = gatorImage[i];     // 0=black, 1=transparent
    rgba[4*i+0]=0; rgba[4*i+1]=0; rgba[4*i+2]=0; rgba[4*i+3]= bit?0:255;
  }
  copyToCanvas(gatorWidth, gatorHeight, rgba.data(), id);
}

static void finalizeImageLoad(emscripten_fetch_t* res){
  std::string id = res->userData ? std::string((char*)res->userData) : "urlCanvas";
  if (res->status==200 && res->numBytes>=12){
    const uint8_t* p = (const uint8_t*)res->data;
    if (std::memcmp(p,"RGBA",4)==0){
      auto be32=[](const uint8_t* x)->uint32_t{
        return (uint32_t(x[0])<<24)|(uint32_t(x[1])<<16)|(uint32_t(x[2])<<8)|uint32_t(x[3]);
      };
      uint32_t w=be32(p+4), h=be32(p+8);
      const uint8_t* pixels = p+12;
      if (res->numBytes >= 12 + (int64_t)w*h*4){
        copyToCanvas((int)w,(int)h,pixels,id);
      }
    }
  }
  emscripten_fetch_close(res);
  free(res->userData);
}

void initiateImageLoad(std::string url, std::string id){
  char* saved=(char*)malloc(id.size()+1);
  std::memcpy(saved,id.data(),id.size()); saved[id.size()]=0;
  emscripten_fetch_attr_t a; emscripten_fetch_attr_init(&a);
  std::strcpy(a.requestMethod,"GET");
  a.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
  a.onsuccess = finalizeImageLoad;
  a.onerror   = finalizeImageLoad;
  a.userData  = saved;
  emscripten_fetch(&a, url.c_str());
}

EMSCRIPTEN_BINDINGS(mod){
  emscripten::function("displayHeaderImage",&displayHeaderImage);
  emscripten::function("initiateImageLoad",&initiateImageLoad);
}
