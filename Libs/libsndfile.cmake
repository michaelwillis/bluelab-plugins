set(BLUELAB_DEPS ${IPLUG2_DIR}/BL-Dependencies)

#
add_library(_libsndfile INTERFACE)
set(LIBSNDFILE_SRC "${BLUELAB_DEPS}/libsndfile/src/")
set(_libsndfile_src
  aiff.c
  alac.c
  alaw.c
  au.c
  audio_detect.c
  avr.c
  broadcast.c
  caf.c
  cart.c
  chanmap.c
  chanmap.h
  chunk.c
  command.c
  common.c
  common.h
  config.h
  dither.c
  double64.c
  dwd.c
  dwvw.c
  file_io.c
  flac.c
  float32.c
  g72x.c
  gsm610.c
  htk.c
  id3.c
  ima_adpcm.c
  ima_oki_adpcm.c
  ima_oki_adpcm.h
  interleave.c
  ircam.c
  macos.c
  mat4.c
  mat5.c
  mpc2k.c
  ms_adpcm.c
  nist.c
  ogg.c
  ogg.h
  ogg_opus.c
  ogg_pcm.c
  ogg_speex.c
  ogg_vorbis.c
  paf.c
  pcm.c
  pvf.c
  raw.c
  rf64.c
  rx2.c
  sd2.c
  sds.c
  sfconfig.h
  sfendian.h
  sf_unistd.h
  sndfile.c
  sndfile.h
  strings.c
  svx.c
  txw.c
  ulaw.c
  voc.c
  vox_adpcm.c
  w64.c
  wav.c
  wavlike.c
  wavlike.h
  windows.c
  wve.c
  xi.c
  ALAC/ag_dec.c
  ALAC/ag_enc.c
  ALAC/aglib.h
  ALAC/ALACAudioTypes.h
  ALAC/ALACBitUtilities.c
  ALAC/ALACBitUtilities.h
  ALAC/alac_codec.h
  ALAC/alac_decoder.c
  ALAC/alac_encoder.c
  ALAC/dp_dec.c
  ALAC/dp_enc.c
  ALAC/dplib.h
  ALAC/EndianPortable.h
  ALAC/matrix_dec.c
  ALAC/matrix_enc.c
  ALAC/matrixlib.h
  ALAC/shift.h
  G72x/g721.c
  G72x/g723_16.c
  G72x/g723_24.c
  G72x/g723_40.c
  G72x/g72x.c
  G72x/g72x.h
  G72x/g72x_priv.h
  GSM610/add.c
  GSM610/code.c
  GSM610/config.h
  GSM610/decode.c
  GSM610/gsm610_priv.h
  GSM610/gsm_create.c
  GSM610/gsm_decode.c
  GSM610/gsm_destroy.c
  GSM610/gsm_encode.c
  GSM610/gsm.h
  GSM610/gsm_option.c
  GSM610/long_term.c
  GSM610/lpc.c
  GSM610/preprocess.c
  GSM610/rpe.c
  GSM610/short_term.c
  GSM610/table.c
  )
list(TRANSFORM _libsndfile_src PREPEND "${LIBSNDFILE_SRC}")
iplug_target_add(_libsndfile INTERFACE
  INCLUDE ${BLUELAB_DEPS}/libsndfile/src
  SOURCE ${_libsndfile_src}
)