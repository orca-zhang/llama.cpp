#pragma once

#include <cstdint>

struct ggml_cgraph;
struct ggml_context;
struct ggml_tensor;
struct llama_ubatch;

// TODO: pass to llama_model graph build
class llama_graph_i {
public:
    // apply control vector for layer il
    virtual ggml_tensor * build_cvec(
            ggml_context * ctx0,
             ggml_tensor * cur,
                     int   il) = 0;

    // do mat_mul, while optionally apply lora
    virtual ggml_tensor * build_lora_mm(
            ggml_context * ctx0,
             ggml_tensor * w,
             ggml_tensor * cur) = 0;

    // do mat_mul_id, while optionally apply lora
    virtual ggml_tensor * build_lora_mm_id(
            ggml_context * ctx0,
             ggml_tensor * w,   // struct ggml_tensor * as
             ggml_tensor * cur, // struct ggml_tensor * b
             ggml_tensor * ids) = 0;

    virtual ggml_tensor * build_rope_factors(int il) = 0;

    // graph build API (context-specific)

    virtual ggml_tensor * build_inp_embd(
            ggml_context * ctx0,
             ggml_tensor * tok_embd,
      const llama_ubatch & ubatch) = 0;

    virtual ggml_tensor * build_inp_pos(
            ggml_context * ctx0,
                 int32_t   n_tokens) = 0;

    virtual ggml_tensor * build_inp_out_ids(
            ggml_context * ctx0,
                 int32_t   n_tokens,
                    bool   worst_case) = 0;

    virtual ggml_tensor * build_inp_mean(
            ggml_context * ctx0,
                 int32_t   n_tokens) = 0;

    virtual ggml_tensor * build_inp_cls(
            ggml_context * ctx0,
                 int32_t   n_tokens) = 0;

    virtual void build_attn_inp(
            ggml_context * ctx0,
                 int32_t   n_tokens,
                    bool   causal,
                    bool   swa,
                    bool   worst_case) = 0;

    virtual void build_attn_kv_store(
            ggml_context * ctx0,
             ggml_cgraph * graph,
             ggml_tensor * k_cur,
             ggml_tensor * v_cur,
                 int32_t   n_tokens,
                 int64_t   il,
                 bool      worst_case) = 0;

    virtual ggml_tensor * build_attn_qkv(
            ggml_context * ctx0,
             ggml_cgraph * graph,
             ggml_tensor * wo,
             ggml_tensor * wo_b,
             ggml_tensor * q_cur,
                 int32_t   n_tokens,
                 float     kq_scale,
                 int       il,
                 bool      worst_case) = 0;

    virtual ggml_tensor * build_soft_max_ext(
            ggml_context * ctx0,
             ggml_tensor * kq,
                 float     kq_scale) = 0;

    virtual void build_k_shift(
            ggml_context * ctx0,
             ggml_cgraph * graph) = 0;

    // find holes from the beginning of the KV cache and fill them by moving data from the end of the cache
    virtual void build_defrag(
            ggml_context * ctx0,
             ggml_cgraph * graph) = 0;

    virtual ggml_tensor * build_inp_embd_enc(
            ggml_context * ctx0,
                 int32_t   n_tokens,
                    bool   worst_case) = 0;

    virtual ggml_tensor * build_inp_KQ_mask_cross(
            ggml_context * ctx0,
                 int32_t   n_tokens,
                    bool   worst_case) = 0;

    virtual ggml_tensor * build_inp_s_copy(
            ggml_context * ctx0,
                    bool   worst_case) = 0;

    virtual ggml_tensor * build_inp_s_mask(
            ggml_context * ctx0,
                    bool   worst_case) = 0;

    virtual ggml_tensor * build_copy_mask_state(
            ggml_context * ctx0,
             ggml_cgraph * graph,
             ggml_tensor * s,
             ggml_tensor * state_copy,
             ggml_tensor * state_mask,
                 int32_t   n_tokens,
                 int32_t   n_state,
                 int32_t   n_seqs,
                    bool   worst_case) = 0;

    virtual ggml_tensor * build_mamba_layer(
            ggml_context * ctx0,
             ggml_cgraph * graph,
             ggml_tensor * cur,
             ggml_tensor * state_copy,
             ggml_tensor * state_mask,
      const llama_ubatch & ubatch,
                     int   il,
                    bool   worst_case) = 0;

    virtual ggml_tensor * build_rwkv_token_shift_load(
            ggml_context * ctx0,
             ggml_cgraph * graph,
             ggml_tensor * state_copy,
             ggml_tensor * state_mask,
      const llama_ubatch & ubatch,
                     int   il,
                    bool   worst_case) = 0;

    virtual ggml_tensor * build_rwkv_token_shift_store(
            ggml_context * ctx0,
             ggml_tensor * token_shift,
      const llama_ubatch & ubatch,
                     int   il,
                    bool   worst_case) = 0;

    virtual ggml_tensor * build_rwkv6_time_mix(
            ggml_context * ctx0,
             ggml_cgraph * graph,
             ggml_tensor * cur,
             ggml_tensor * x_prev,
             ggml_tensor * state_copy,
             ggml_tensor * state_mask,
      const llama_ubatch & ubatch,
                     int   il,
                    bool   worst_case) = 0;
};
