#include <HTMLViz.h>

#include <algorithm>

#define STEPS_PER_SEC 15

std::string sanitize_string(const std::string& str);

const char* start_css = R"(<!DOCTYPE html>
<html>
<head>
<style>
.grid-container {
  display: grid;
)";

const char* css_content = R"(
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
    resize: none;
    user-select: none;
}

.sp {
  background-color: green;
  grid-column: 1;
  grid-row: 1;
  font-size: 8px;
  text-align: center;
  z-index: 10;
  transition: 1s;
  margin-right: -0%;
  margin-left: 0%;
  margin-top: 0%;
  margin-bottom: -0%;
  border-radius: 10%;
}

.output {
  content: "-";
  white-space: pre;
}
)";

const char* start_output_anim = R"(
@keyframes output-anim {
  0% {
    content: "";
  }
)";

const char* start_body = R"(
}

</style>
</head>
<body>

<div class="grid-container">
)";

const char* end_body = R"(
</div>

<div>
<p>OUTPUT: <span class="output"></span></p>
</div>

</body>
</html>
)";

namespace Ircis {
  void HTMLVizCSS::generate_html() {
    size_t num_rows = lines_.size();
    size_t num_cols = lines_[0].size();

    output_file_ << start_css;
    output_file_ << "grid-template-columns: repeat(" << num_cols << ", auto);\n";
    output_file_ << css_content;

    size_t runner_no = 0;
    size_t max_path_length = std::max_element(paths_.begin(), paths_.end(),
					      [](auto& lhs, auto& rhs) {
						return lhs.path.size() < rhs.path.size();
					      })->path.size();
    float percent_inc = 100.0/max_path_length;
    for (const auto& data: paths_) {
      output_file_ << ".num" << runner_no << " {\n";
      output_file_ << "  animation: anim" << runner_no << " " << max_path_length/STEPS_PER_SEC << "s forwards;\n";
      output_file_ << "}\n";
      output_file_ << "@keyframes anim" << runner_no << " {\n";
      float current_percent = 0;
      for (size_t ii = 0; ii < data.path.size(); ++ii) {
	auto& pos = data.path[ii];
      // for (auto& pos: data.path) {
	output_file_ << current_percent << "% {\n";
	output_file_ << "  margin-right: -" << pos.get_x() << "00%;\n";
	output_file_ << "  margin-left: " << pos.get_x() << "00%;\n";
	output_file_ << "  margin-top: " << pos.get_y() << "00%;\n";
	output_file_ << "  margin-bottom: -" << pos.get_y() << "00%;\n";
	if (ii >= data.alive_from && ii < data.alive_till)
	  output_file_ << "  opacity: 1;\n";
	else
	  output_file_ << "  opacity: 0;\n";
	output_file_ << "}\n";
	current_percent += percent_inc;
      }
      output_file_ << current_percent << "% {\n";
      output_file_ << "  margin-right: -" << data.path.back().get_x() << "00%;\n";
      output_file_ << "  margin-left: " << data.path.back().get_x() << "00%;\n";
      output_file_ << "  margin-top: " << data.path.back().get_y() << "00%;\n";
      output_file_ << "  margin-bottom: -" << data.path.back().get_y() << "00%;\n";
      output_file_ << "  opacity: 0;\n";
      output_file_ << "}\n";
      output_file_ << "100% {\n";
      output_file_ << "  margin-right: -" << data.path.back().get_x() << "00%;\n";
      output_file_ << "  margin-left: " << data.path.back().get_x() << "00%;\n";
      output_file_ << "  margin-top: " << data.path.back().get_y() << "00%;\n";
      output_file_ << "  margin-bottom: -" << data.path.back().get_y() << "00%;\n";
      output_file_ << "  opacity: 0;\n";
      output_file_ << "}\n";
      output_file_ << "}\n";
      ++runner_no;
    }

    output_file_ << ".output:after {\n";
    output_file_ << "  animation: output-anim " << max_path_length/STEPS_PER_SEC << "s forwards;\n";
    output_file_ << "  content: \"--\";\n";
    output_file_ << "}\n";
    output_file_ << start_output_anim;
    std::string prev_output = "";
    for (auto& data: time_output_data_) {
      std::string sanitized_content = sanitize_string(data.second);
      output_file_ << data.first*percent_inc-0.1 << "% {\n";
      output_file_ << R"(  content: ")" << prev_output << "\";\n";
      output_file_ << R"(  background: white;\n)";
      output_file_ << "}\n";
      output_file_ << data.first*percent_inc << "% {\n";
      output_file_ << R"(  content: ")" << sanitized_content << "\";\n";
      output_file_ << R"(  background: black;\n)";
      output_file_ << "}\n";
      output_file_ << data.first*percent_inc+0.1 << "% {\n";
      output_file_ << R"(  content: ")" << sanitized_content << "\";\n";
      output_file_ << R"(  background: white;\n)";
      output_file_ << "}\n";
      prev_output = sanitized_content;
    }
    output_file_ << "100% {\n";
    output_file_ << R"(  content: ")" << prev_output << "\";\n";
    output_file_ << R"(  background: white;\n)";
    output_file_ << "}\n";
    output_file_ << "}\n";

    output_file_ << start_body;
    for (size_t xx = 0; xx < num_rows; ++xx) {
      for (size_t yy = 0; yy < num_cols; ++yy) {
	output_file_ << R"(<div class="grid-item" style="grid-row:)" << xx+1 << R"(; grid-column:)" << yy+1 << R"("><textarea class="text" readonly>)" << lines_[xx][yy] << "</textarea></div>\n";
      }
    }

    for (size_t runner_no = 0; runner_no < paths_.size(); ++runner_no) {
      output_file_ << R"(<div class="num)" << runner_no << R"( sp"></div>)" << '\n';
    }

    output_file_ << end_body;
  }
}

std::string sanitize_string(const std::string& str) {
  std::string clean_str(str);
  while(true) {
    auto it = clean_str.find("\n");
    if (it == std::string::npos)
      break;
    clean_str.replace(it, 1, "\\A ");
  }
  return clean_str;
}
