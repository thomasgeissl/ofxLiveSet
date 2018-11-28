import React, { Component } from 'react';
import { connect } from "react-redux";

const mapStateToProps = state => {
    return { tracks: state.session.tracks };
};

class Clip extends Component {
  render() {
    return (
      <div className="Clip">
        clip
      </div>
    );
  }
}

export default connect(mapStateToProps)(Clip);