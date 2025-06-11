/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fill.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 21:09:40 by jaubry--          #+#    #+#             */
/*   Updated: 2025/06/11 23:17:31 by jaubry--         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font_renderer.h"

/**
 * @brief Calculate winding direction for edge from p1 to p2
 */
static int get_edge_winding(t_vec2 p1_pt, t_vec2 p2_pt)
{
    if (p1_pt.y < p2_pt.y)
        return (1);   // Going down = positive winding
    else if (p1_pt.y > p2_pt.y)
        return (-1);  // Going up = negative winding
    return (0);       // Horizontal edge - ignore
}

/**
 * @brief Create edge from two points
 */
static t_edge *create_edge(t_vec2 p1_pt, t_vec2 p2_pt)
{
    t_edge *edge;
    int winding;

    winding = get_edge_winding(p1_pt, p2_pt);
    if (winding == 0)
        return (NULL);  // Skip horizontal edges
    
    edge = ft_calloc(1, sizeof(t_edge));
    if (!edge)
        return (NULL);
    
    if (winding > 0)
    {
        edge->ymin = (int)p1_pt.y;
        edge->ymax = (int)p2_pt.y;
        edge->x_current = p1_pt.x;
        edge->dx = (p2_pt.x - p1_pt.x) / (p2_pt.y - p1_pt.y);
    }
    else
    {
        edge->ymin = (int)p2_pt.y;
        edge->ymax = (int)p1_pt.y;
        edge->x_current = p2_pt.x;
        edge->dx = (p1_pt.x - p2_pt.x) / (p1_pt.y - p2_pt.y);
    }
    edge->winding = winding;
    return (edge);
}

/**
 * @brief Add edge to active list in sorted order by x_current
 */
static void insert_active_edge(t_fill_context *ctx, t_edge *new_edge)
{
    t_edge **current;

    current = &ctx->aet;
    while (*current && (*current)->x_current < new_edge->x_current)
        current = &(*current)->next;
    new_edge->next = *current;
    *current = new_edge;
}

/**
 * @brief Remove edges that end at current scanline
 */
static void remove_finished_edges(t_fill_context *ctx)
{
    t_edge **current;

    current = &ctx->aet;
    while (*current)
    {
        if ((*current)->ymax <= ctx->scanline_y)
        {
            t_edge *to_remove = *current;
            *current = (*current)->next;
            free(to_remove);
        }
        else
        {
            (*current)->x_current += (*current)->dx;
            current = &(*current)->next;
        }
    }
}

/**
 * @brief Fill horizontal line using non-zero winding rule
 */
static void fill_horizontal_line(t_env *env, t_fill_context *ctx)
{
    t_edge *edge;
    int winding_count;
    int fill_start;
    int current_x;
    int screen_y;

    edge = ctx->aet;
    winding_count = 0;
    fill_start = -1;
    screen_y = ctx->position.y + ctx->scanline_y;
    
    while (edge)
    {
        current_x = (int)edge->x_current;
        
        if (winding_count == 0 && fill_start == -1)
            fill_start = current_x;
        
        winding_count += edge->winding;
        
        if (winding_count == 0 && fill_start != -1)
        {
            // Fill from fill_start to current_x
            ft_mlx_horizontal_line(&env->mlx->img, 
                ctx->position.x + fill_start,
                ctx->position.x + current_x,
                screen_y, ctx->fill_color);
            fill_start = -1;
        }
        edge = edge->next;
    }
}

/**
 * @brief Process contour and collect edges for a single contour
 */
static void collect_contour_edges(t_fill_context *ctx, int start_idx, int end_idx)
{
    int curr_idx;
    int next_idx;
    t_vec2 curr_pt;
    t_vec2 next_pt;
    t_edge *edge;

    curr_idx = start_idx;
    while (curr_idx <= end_idx)
    {
        next_idx = (curr_idx == end_idx) ? start_idx : curr_idx + 1;
        
        curr_pt = new_vec2(ctx->glyf->x_coordinates[curr_idx],
                          ctx->glyf->y_coordinates[curr_idx]);
        next_pt = new_vec2(ctx->glyf->x_coordinates[next_idx],
                          ctx->glyf->y_coordinates[next_idx]);
        
        // Apply transformation if present
        curr_pt = apply_transform(curr_pt, ctx->transform);
        next_pt = apply_transform(next_pt, ctx->transform);
        
        // Create edge and add to active list if it intersects current scanline
        edge = create_edge(curr_pt, next_pt);
        if (edge && edge->ymin <= ctx->scanline_y && edge->ymax > ctx->scanline_y)
        {
            // Update x_current for current scanline
            edge->x_current += edge->dx * (ctx->scanline_y - edge->ymin);
            insert_active_edge(ctx, edge);
        }
        else if (edge)
            free(edge);
        
        curr_idx++;
    }
}

/**
 * @brief Fill a single simple glyph using scanline algorithm
 */
void fill_simple_glyph(t_env *env, t_glyf_table *glyph, t_vec2 pos, 
                      int color, t_glyf_component *transform)
{
    t_fill_context ctx;
    int contour_idx;
    int start_idx;
    int end_idx;
    int y_min;
    int y_max;

    if (!glyph || glyph->header->number_of_contours <= 0)
        return;
    
    // Initialize context
    ctx.glyf = glyph;
    ctx.aet = NULL;
    ctx.fill_color = color;
    ctx.position = pos;
    ctx.transform = transform;
    
    // Use glyph bounding box for scanline range
    y_min = glyph->header->y_min;
    y_max = glyph->header->y_max;
    
    // Process each scanline
    ctx.scanline_y = y_min;
    while (ctx.scanline_y < y_max)
    {
        // Collect edges from all contours that intersect this scanline
        contour_idx = 0;
        while (contour_idx < glyph->header->number_of_contours)
        {
            start_idx = (contour_idx == 0) ? 0 : glyph->end_pts[contour_idx - 1] + 1;
            end_idx = glyph->end_pts[contour_idx];
            collect_contour_edges(&ctx, start_idx, end_idx);
            contour_idx++;
        }
        
        // Fill the scanline if we have active edges
        if (ctx.aet)
            fill_horizontal_line(env, &ctx);
        
        // Update edges for next scanline
        remove_finished_edges(&ctx);
        ctx.scanline_y++;
    }
    
    // Clean up any remaining edges
    while (ctx.aet)
    {
        t_edge *temp = ctx.aet;
        ctx.aet = ctx.aet->next;
        free(temp);
    }
}
