#include <HTMLViz.h>

#define STEP_TIME 0.35

static const char* start_css = R"(<!DOCTYPE html>
<html>
<head>
<style>
.grid-container {
  display: grid;
)";

static const char* end_css = R"(
  background-color: #2196F3;
  grid-auto-rows: 1fr;
  padding: 10px;
}

.invis-boi {
    /* border: 1px solid rgba(0, 0, 0, 0.1); */
    width: 0%;
    padding-bottom: 100%;
}
.grid-item {
    position: relative;
    background-color: rgba(255, 255, 255, 0.8);
    /* border: 1px solid rgba(0, 0, 0, 0.1); */
    width: 100%;
    padding-top: 100%;
}
.text {
    position: absolute;
    top: -25%;
    padding-top: 50%;
    height: 100%;
    width: 100%;
    font-size: 1vw; /*have to find a good value for this*/
    text-align: center;
    color: black;
    background: none;
    border: none;
}

.sp {
  background-color: green;
  grid-column: 1;
  grid-row: 1;
  font-size: 8px;
  text-align: center;
  z-index: 10;
  transition: 1s;
  border-radius: 10%;
}

</style>
</head>
<body>

<div class="grid-container">
)";

static const char* start_script = R"(
</div>
<script type='text/javascript' src="html/gsap.min.js"></script>
<script>
var val = document.querySelector(".sp").getBoundingClientRect().width;
)";

static const char* end_body = R"(
</script>

</body>
</html>
)";


namespace Ircis {
  void HTMLVizJS::generate_html() {
    size_t num_rows = lines_.size();
    size_t num_cols = lines_[0].size();

    output_file_ << start_css;
    output_file_ << "grid-template-columns: repeat(" << num_cols << ", auto);\n";
    output_file_ << end_css;


    for (size_t xx = 0; xx < num_rows; ++xx) {
      for (size_t yy = 0; yy < num_cols; ++yy) {
	output_file_ << R"(<div class="grid-item" style="grid-row:)" << xx+1 << R"(; grid-column:)" << yy+1 << R"("><textarea class="text">)" << lines_[xx][yy] << "</textarea></div>\n";
      }
    }

    for (size_t runner_no = 0; runner_no < paths_.size(); ++runner_no) {
      output_file_ << R"(<div class="num)" << runner_no << R"( sp"></div>)" << '\n';
    }
    output_file_ << start_script;

    size_t runner_no = 0;
    for (const auto& data: paths_) {
      // var tl1 = gsap.timeline({repeat: 30, repeatDelay: 1});
      // tl1.to(".jsanim", {marginTop:'0%', marginbottom:'-0%', marginleft:'0%', marginright:'-0%'}, "+=1")
      output_file_ << "var tl" << runner_no << " = gsap.timeline({repeat: 30, repeatDelay: 1});\n";
      output_file_ << "tl" << runner_no << R"(.to(".num)" << runner_no << R"(", {marginTop:'0',marginBottom:'-0',marginLeft:'0',marginRight:'-0'},"+=1"))"<<'\n';
      for (size_t ii = 0; ii < data.path.size(); ++ii) {
	// .to(".jsanim", {marginTop:'100%', marginBottom:'-100%', marginLeft:'100%', marginRight:'-100%'}, "+=0.75")
	auto& pos = data.path[ii];
	output_file_ << R"(.to(".num)" << runner_no << R"(", {marginTop:)"<<pos.get_y()<<R"(*val+'px',marginBottom:-)"<<pos.get_y()<<R"(*val+'px',marginLeft:)"<<pos.get_x()<<R"(*val+'px',marginRight:-)"<<pos.get_x()<<R"(*val+'px',ease: "power2.out"},"+=)"<<STEP_TIME<<R"("))"<<'\n';
      }
      output_file_ << R"(.to(".num)" << runner_no << R"(", {opacity:0},"+=1");)"<<'\n';
    }

    output_file_ << end_body;
  }
}
